#include "mobiledevice.hpp"

MobileDevice::MobileDevice(QObject *parent) :
    QObject(parent)
{
    qWarning() << "Usage error: Can't construct a MobileDevice independently. Use MobileDevice::getDevices() instead.";
}

MobileDevice::MobileDevice(LIBMTP_mtpdevice_t *inDevice) {
    device = inDevice;
}

int MobileDevice::getBatteryLevel() {
    uint8_t max = 100;
    uint8_t actual;
    LIBMTP_Get_Batterylevel(device, &max, &actual);
    return actual;
}

void MobileDevice::uploadCSVFile(QFile *file) {

    file->open(QIODevice::ReadOnly | QIODevice::Text);

    //TODO
    LIBMTP_file_t *remoteFile;

}

void MobileDevice::uploadCSVText(QString *text) {

    //TODO
    LIBMTP_file_t *remoteFile;
}

QString *MobileDevice::getJsonText() {
    LIBMTP_file_t *remoteFile;
    //TODO

    QString *text = new QString();

    return text;
}

//Static method
QList<MobileDevice *> *MobileDevice::getDevices() {

    LIBMTP_mtpdevice_t *devices, *iterator;

    switch(LIBMTP_Get_Connected_Devices(&devices)) {
    case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
        qDebug() << "No device attached! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_CONNECTING:
        qDebug() << "MTP connection error! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_MEMORY_ALLOCATION:
        qDebug() << "MTP memory allocation error! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_PTP_LAYER:
        qDebug() << "PTP layer error! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_USB_LAYER:
        qDebug() << "USB layer error! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_STORAGE_FULL:
        qDebug() << "Storage full error! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_CANCELLED:
        qDebug() << "Operation canceled! exiting.";
        exit(1);
        break;
    case LIBMTP_ERROR_GENERAL:
        qDebug() << "General error with libmtp! exiting.";
        exit(1);
        break;

    case LIBMTP_ERROR_NONE:
        qDebug() << "Successfully got devices.";
    }

    QList<MobileDevice *> *deviceList = new QList<MobileDevice *>();
    //Iterate through the devices and add each to the list
    for(iterator = devices; iterator != NULL; iterator = iterator->next) {
        LIBMTP_Dump_Errorstack(iterator);
        LIBMTP_Clear_Errorstack(iterator);

        deviceList->append(new MobileDevice(iterator));
    }

    return deviceList;
}

QString *MobileDevice::toString() {
    QString *string = new QString();
    (*string) += LIBMTP_Get_Modelname(device);
    return string;
}
