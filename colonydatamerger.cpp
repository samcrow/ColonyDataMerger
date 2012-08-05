#include "colonydatamerger.hpp"
#include "ui_colonydatamerger.h"

//The number of connected devices
uint32_t deviceCount = 0;

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
