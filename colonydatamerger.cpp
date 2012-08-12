#include "colonydatamerger.hpp"
#include "ui_colonydatamerger.h"


ColonyDataMerger::ColonyDataMerger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColonyDataMerger)
{
    ui->setupUi(this);
    qInstallMsgHandler(&handleConsoleMessage);
    //Fix warning about handleConsoleMessage defined but not used
    handleConsoleMessage(QtDebugMsg, "Starting up");

    LIBMTP_Init();

    qDebug() << "It works!";
}

void ColonyDataMerger::showConsoleDialog() {
    if(!activeDialog) {
        activeDialog = new ConsoleDialog(this);
    }
    activeDialog->show();
    activeDialog->raise();
    activeDialog->activateWindow();
}

QList<Colony *> *ColonyDataMerger::mergeColonyLists(QList<Colony *> *list1, QList<Colony *> *list2) {

    //TODO sometime: Find a better algorithm. This one is not very scalable (probably O(n^2))

    QList<Colony *> *finalList = new QList<Colony *>();

    //Add every colony from both lists to the final list
    for(int i = 0, max = list1->length(); i < max; i++) {
        finalList->append(list1->at(i));
    }
    for(int i = 0, max = list2->length(); i < max; i++) {
        finalList->append(list2->at(i));
    }

    //Go through the list and remove duplicates
    for(int i = 0, max = finalList->length(); i < max; i++) {
        Colony *flColony = finalList->at(i);
        int flId = flColony->getID();

        //Try to find other colonies in the list with this ID
        for(int j = 0, jmax = finalList->length(); j < jmax; j++) {
            Colony *innerFlColony = finalList->at(i);

            //If another colony was found with the same ID
            // (that is not the same colony object as this one
            if(innerFlColony->getID() == flId && innerFlColony != flColony) {
                //We have two colony pointers, same ID, one of them is better

                Colony *chosenColony = chooseColony(flColony, innerFlColony);

                //chosenColony is either flColony or innerFlColony
                //Remove from the final list the one that was not chosen
                if(chosenColony == flColony) {
                    finalList->removeOne(innerFlColony);
                }
                else {
                    //chosenColony == innerFlColony
                    finalList->removeOne(flColony);
                }
            }
        }
    }

    return finalList;
}

Colony *ColonyDataMerger::chooseColony(Colony *colony1, Colony *colony2) {
    QDateTime date1 = colony1->getModifiedDate();
    QDateTime date2 = colony2->getModifiedDate();

    //Case 0: both are valid
    if(date1.isValid() && date2.isValid()) {
        //Colony 1 updated after colony 2
        if(date1 > date2) {
            return colony1;
        }
        //Colony 1 updated before colony 2
        else if(date1 < date2) {
            return colony2;
        }
        //Same date/time, arbitrarily return colony 1
        else {
            return colony1;
        }
    }

    //Case 1: One date is valid; return the colony with the valid date
    else if(date1.isValid() && !date2.isValid()) {
        return colony1;
    }
    else if(date2.isValid() && !date1.isValid()) {
        return colony2;
    }

    //Case 2: neither is valid
    return colony1;
}

ColonyDataMerger::~ColonyDataMerger()
{
    delete ui;
}

void ColonyDataMerger::on_uploadButton_clicked()
{
    showConsoleDialog();
    //Disable the upload button so it can't be clicked again this time the application is run
    ui->uploadButton->setEnabled(false);

    //Upload the file

    QList<MobileDevice *> *devices = MobileDevice::getDevices();

    if(!devices) {
        qWarning() << "NULL mobile device set! Exiting.";
        return;
    }

    qDebug() << "Connected to" << devices->count() << "device(s).";

    QString fileName = QFileDialog::getOpenFileName(activeDialog, "Choose a CSV file to upload", QDir::homePath(), "CSV files (*.csv)");

    qDebug() << "CSV file:" << fileName;

    if(fileName.isEmpty()) {
        qWarning() << "No CSV file selected. Exiting.";
        return;
    }

    foreach(MobileDevice *device, *devices) {
        QFile csv(fileName);
        csv.open(QIODevice::ReadOnly | QIODevice::Text);
        if(!csv.isOpen()) {
            qWarning() << "Could not open CSV file. Exiting.";
            return;
        }

        qDebug() << "Uploading to a device...";
        device->uploadCSV(&csv);

        if(csv.isOpen()) {
            csv.close();
        }
    }

    //Clean up
    foreach(MobileDevice *device, *devices) {
        delete device;
    }
    delete devices;
    qDebug() << "Operation successfully completed.";
}

void ColonyDataMerger::on_downloadButton_clicked()
{
    showConsoleDialog();
    ui->downloadButton->setEnabled(false);

    QList<MobileDevice *> *devices = MobileDevice::getDevices();

    if(!devices) {
        qWarning() << "NULL mobile device set! Exiting.";
        return;
    }
    qDebug() << "Connected to" << devices->count() << "device(s).";

    foreach(MobileDevice *device, *devices) {

        QString jsonText = device->getJsonText();

        QString fileName = QFileDialog::getSaveFileName(this, "Choose where to save this JSON file", QDir::homePath(), "JSON files (*.json)");

        if(fileName.isEmpty()) {
            qDebug() << "Emtpy file name. Exiting.";
            return;
        }

        QFile file(fileName);
        file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
        if(!file.isOpen()) {
            qDebug() << "File opening failed. Exiting.";
            return;
        }
        if(!file.isWritable()) {
            qDebug() << "File is not writable. Exiting.";
        }

        file.write(jsonText.toUtf8());
        file.close();

        qDebug() << "Done writing file" << file.fileName();
    }

    //Clean up
    foreach(MobileDevice *device, *devices) {
        delete device;
    }
    delete devices;
    qDebug() << "Operation successfully completed.";
}

void ColonyDataMerger::on_mergeButton_clicked()
{
    showConsoleDialog();

    QStringList fileNames = QFileDialog::getOpenFileNames(this, "Choose JSON files to merge & export", QDir::homePath(), "JSON files (*.json)");

    QList<QList<Colony *> *> colonyLists;

    foreach(QString fileName, fileNames) {
        qDebug() << "Opening" << fileName;
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        if(!file.isOpen()) {
            qDebug() << "File opening failed. Exiting.";
            return;
        }

        QList<Colony *> *thisFileList = jsonToColonyList(parseJson(QString(file.readAll())));

        colonyLists.append(thisFileList);
    }

    QList<Colony *> *workingList = new QList<Colony *>();

    qDebug() << "Merging colony lists...";
    for(int i = 0, max = colonyLists.length(); i < max; i++ ) {
        QList<Colony *> *list = colonyLists[i];
        workingList = mergeColonyLists(list, workingList);
    }

    qDebug() << "Merging done.";
    //Convert it to CSV
    QString csvText = toCSV(workingList);

    QString csvPath = QFileDialog::getSaveFileName(this, "Choose where to export the CSV file", QDir::homePath(), "CSV files (*.csv)");

    if(!csvPath.isEmpty()) {

        QFile csv(csvPath);
        csv.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);

        csv.write(csvText.toUtf8());
        csv.close();
        qDebug() << "Done writing CSV file.";
    }

    //Clean up
    delete workingList;
    QListIterator<QList<Colony *> *> listsIterator(colonyLists);
    while(listsIterator.hasNext()) {
        QList<Colony *> *list = listsIterator.next();
        QListIterator<Colony *> colonyIterator(*list);
        while(colonyIterator.hasNext()) {
            delete colonyIterator.next();
        }
        delete list;
    }
    qDebug() << "Operation successfully completed.";
}

QList<Colony *> *ColonyDataMerger::jsonToColonyList(QVariantMap json) {

    QVariantList jsonColonies = json.value("colonies").toList();

    QList<Colony *> *colonies = new QList<Colony *>();

    for(int i = 0, max = jsonColonies.length(); i < max; i++) {
        QVariant jsonColony = jsonColonies.at(i);
        QVariantMap colonyMap = jsonColony.toMap();

        Colony *colony = new Colony();
        colony->fromVariant(colonyMap);

        //Ensure that the colony's marked as visited if it is active
        if(colony->isActive()) {
            colony->setVisited(true);
        }

        colonies->append(colony);
    }

    qDebug() << "Got" << colonies->length() << "colonies.";
    return colonies;
}

QVariantMap ColonyDataMerger::parseJson(QString jsonText) {

    QJson::Parser parser;

    QVariant variant = parser.parse(jsonText.toUtf8());

    return variant.toMap();
}

QString ColonyDataMerger::toCSV(QList<Colony *> *list) {
    QString csv;

    csv += "col,E,N,active,visted,\n";

    for(int i = 0, max = list->size(); i < max; i++) {
        Colony *colony = list->at(i);

        if(colony->isVisited()) {

            csv += QString::number(colony->getID());
            csv += ",";
            csv += QString::number(colony->getX());
            csv += ",";
            csv += QString::number(colony->getY());
            csv += ",";
            csv += colony->isActive() ? "A" : "NA";
            csv += ",";
            csv += colony->isVisited() ? "true" : "false";
            csv += ",\n";
        }
    }

    return csv;
}
