#include "colonydatamerger.hpp"
#include "ui_colonydatamerger.h"


ColonyDataMerger::ColonyDataMerger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColonyDataMerger)
{
    ui->setupUi(this);

    LIBMTP_Init();
    QList<MobileDevice *> *devices = MobileDevice::getDevices();

    MobileDevice *firstDevice = devices->at(0);

    qDebug() << "Device:" << firstDevice->toString();

    QString string = "Test";
    firstDevice->uploadCSV(&string);
    QString string2 = " ...And Justice For All";
    firstDevice->uploadCSV(&string2);

    qDebug() << "It works!";

    delete devices;
}

QList<Colony *> *ColonyDataMerger::mergeColonyLists(QList<Colony *> *list1, QList<Colony *> *list2) {

    //TODO sometime: Find a better algorithm. This one is not very scalable (probably O(n^2))

    QList<Colony *> *finalList = new QList<Colony *>();

    //Iterate through list 1
    foreach(Colony *list1colony, *list1) {

        //Check if this colony in list 1 is in list 2
        foreach(Colony *list2colony, *list2) {
            if(list1colony->equals(list2colony)) {
                //Colony is in lists 1 and 2: Get the latest one

                Colony *latestColony = chooseColony(list1colony, list2colony);

                //Add it to the final list
                finalList->append(latestColony);

                break; // Break out into the foreach(..., list1) loop
            }

            //Same colony not found in list 2
            //Add it from list 1 to the master list
            finalList->append(list1colony);
        }
    }

    //Iterate through list 2
    foreach(Colony *list2colony, *list2) {
        //Check if this colony is in list 1

        bool inBoth = false;
        foreach(Colony *list1colony, *list1) {
            if(list1colony->equals(list2colony)) {
                inBoth = true;
            }
        }

        //Insert into the final list each colony that's only in list 2
        if(!inBoth) {
            finalList->append(list2colony);
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
