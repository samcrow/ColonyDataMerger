#include "colonydatamerger.hpp"
#include "ui_colonydatamerger.h"
#include <map>


ColonyDataMerger::ColonyDataMerger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColonyDataMerger)
{
    ui->setupUi(this);
    qInstallMessageHandler(&handleConsoleMessage);

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


    // Create a mapping from colony IDs to Colony instances
    typedef std::map< int, Colony* > map_type;
    map_type colonyMap;
    
    // Add all colonies from list 1 to the map
    for(QList<Colony *>::const_iterator iter = list1->constBegin(); iter != list1->constEnd(); iter++) {
        Colony* colony = *iter;
        colonyMap.insert(std::make_pair(colony->getID(), colony));
    }
    // Add all colonies from list 2 to the map, and check for duplicates
    for(QList<Colony *>::const_iterator iter = list2->constBegin(); iter != list2->constEnd(); iter++) {
        Colony* colony = *iter;
        
        // See if a colony with this ID is already in the map
        map_type::iterator existingIterator = colonyMap.find(colony->getID());
        if(existingIterator == colonyMap.end()) {
            // No colony with this ID in the map
            // Insert this colony
            colonyMap.insert(std::make_pair(colony->getID(), colony));
        }
        else {
            // A colony with this ID already exists
            Colony* existingColony = existingIterator->second;
            
            Colony* chosenColony = chooseColony(existingColony, colony);
            
            // Put the chosen colony in the map
            colonyMap[colony->getID()] = chosenColony;
        }
    }
    
    QList<Colony *> *finalList = new QList<Colony *>();
    finalList->reserve(colonyMap.size());
    // Copy colony pointers from the map into the final list
    for(map_type::iterator iter = colonyMap.begin(); iter != colonyMap.end(); iter++) {
        finalList->append(iter->second);
    }

    return finalList;
}

Colony *ColonyDataMerger::chooseColony(Colony *colony1, Colony *colony2) {
    // Choose whichever is visited
    if(colony1->isVisited() && !colony2->isVisited()) {
        return colony1;
    }
    else if(colony2->isVisited() && !colony1->isVisited()) {
        return colony2;
    }
    else {
        // Both visited
        // If one is active, return that one
        if(colony1->isActive() && !colony2->isActive()) {
            return colony1;
        }
        else if(colony2->isActive() && !colony1->isActive()) {
            return colony2;
        }
        else {
            // Both visited-ness and active-ness are the same
            // Arbitrary
            return colony1;
        }
    }
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
    
    // Write JSON
    QString jsonPath = QFileDialog::getSaveFileName(this, "Choose where to export the JSON file", QDir::homePath(), "JSON files (*.json)");
    if(!jsonPath.isEmpty()) {
        QString jsonText = toJSON(workingList);
        QFile json(jsonPath);
        json.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
        json.write(jsonText.toUtf8());
        json.close();
        qDebug() << "Done writing JSON file.";
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

QList<Colony *> *ColonyDataMerger::jsonToColonyList(QJsonObject json) {

    QJsonArray jsonColonies = json["colonies"].toArray();

    QList<Colony *> *colonies = new QList<Colony *>();

    for(int i = 0, max = jsonColonies.size(); i < max; i++) {
        QJsonObject colonyMap = jsonColonies[i].toObject();

        Colony *colony = new Colony();
        colony->fromVariant(colonyMap.toVariantMap());

        //Ensure that the colony's marked as visited if it is active
        if(colony->isActive()) {
            colony->setVisited(true);
        }

        colonies->append(colony);
    }

    qDebug() << "Got" << colonies->length() << "colonies.";
    return colonies;
}

QJsonObject ColonyDataMerger::parseJson(QString jsonText) {
    return QJsonDocument::fromJson(jsonText.toUtf8()).object();
}

QString ColonyDataMerger::toCSV(QList<Colony *>* list) {
    QString csv;

    csv += "col,E,N,active,visted,\n";

    for(int i = 0, max = list->size(); i < max; i++) {
        Colony *colony = list->at(i);

        csv += QString::number(colony->getID());
        csv += ",";
        csv += QString::number(colony->getX());
        csv += ",";
        csv += QString::number(colony->getY());
        csv += ",";
        csv += colony->isActive() ? "A" : "NA";
        csv += ",";
        csv += colony->isVisited() ? "true" : "false";
        csv += "\n";
    }

    return csv;
}

QString ColonyDataMerger::toJSON(QList<Colony *>* list) {
    QJsonObject root;
    root["comment"] = QStringLiteral("Serialized into JSON by ColonyDataMerger");
    
    QJsonArray colonyArray;
    QList<Colony *>::const_iterator iter = list->constBegin();
    for( ; iter != list->constEnd(); iter++) {
        colonyArray.append(QJsonObject::fromVariantMap((*iter)->toVariant()));
    }
    
    root["colonies"] = colonyArray;
    
    QJsonDocument doc(root);
    return doc.toJson();
}

static void handleConsoleMessage(QtMsgType type, const QMessageLogContext&, const QString& msg) {
    if(activeDialog) {
        activeDialog->appendOutput(type, msg);
    }
    else {
        //Just print it to the standard error stream
        fprintf(stderr, "%s\n", msg.toStdString().c_str());
    }
}
