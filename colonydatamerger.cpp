#include "colonydatamerger.hpp"
#include "ui_colonydatamerger.h"


ColonyDataMerger::ColonyDataMerger(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ColonyDataMerger)
{
    ui->setupUi(this);

    LIBMTP_Init();
    QList<MobileDevice *> *devices = MobileDevice::getDevices();

    qDebug() << "Device:" << devices->at(0)->toString();

    qDebug() << "It works!";
}

ColonyDataMerger::~ColonyDataMerger()
{
    delete ui;
}
