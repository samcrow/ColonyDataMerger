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

void MobileDevice::uploadCSV(QFile *file) {
    //Note: The file should not be closed.

    LIBMTP_file_t *oldFile = getCsvFile();

    if(oldFile == NULL) {
        qWarning() << "No existing CSV file. Stopping upload.";
        return;
    }


    //Remove the CSV file from the device and delete the reference from the heap
    if(LIBMTP_Delete_Object(device, oldFile->item_id) != 0) {

        qWarning() << "Error deleting file with ID" << oldFile->item_id;
    } else {
        qDebug() << "File" << oldFile->item_id << "deleted successfully.";
    }

    //Use the data associated with the old file to upload the new file in its place
    //(except set the file size correctly)
    oldFile->filesize = file->size();

    int handle = file->handle();

    qDebug() << "About to send file with handle" << handle;
    int ret = LIBMTP_Send_File_From_File_Descriptor(device, handle, oldFile, NULL, NULL);
    qDebug() << "Send file returned" << ret;
    LIBMTP_Dump_Errorstack(device);
    LIBMTP_Clear_Errorstack(device);
}

//Currently doesn't work. TODO sometime: fix
void MobileDevice::uploadCSV(QString text) {

    //Write the text to a temporary file so that libmtp can upload it
    QTemporaryFile file;
    file.open();
    file.write(text.toUtf8());//Write that to the file
    file.flush();//Verify that everything's actually been written
    uploadCSV(&file);

}

QString MobileDevice::getJsonText() {

    LIBMTP_file_t *remoteFile = getJsonFile();

    QTemporaryFile file;
    file.open();

    const char *path = file.fileName().toLocal8Bit().data();

    if(LIBMTP_Get_File_To_File(device, remoteFile->item_id, path, NULL, NULL) != 0) {
        qDebug() << "Error getting JSON file!";
        LIBMTP_Dump_Errorstack(device);
        LIBMTP_Clear_Errorstack(device);
    }

    QString text;

    text.append(file.readAll());
    file.close();

    return text;
}

QJsonObject MobileDevice::getJson() {
    QString jsonText = getJsonText();
    return ColonyDataMerger::parseJson(jsonText);
}

QList<Colony *> *MobileDevice::getColonies() {

    QJsonObject json = getJson();

    return ColonyDataMerger::jsonToColonyList(json);
}

LIBMTP_file_t *MobileDevice::getCsvFile() {
    return getFileWithName("colonies.csv");
}

LIBMTP_file_t *MobileDevice::getJsonFile() {
    return getFileWithName("colonies.json");
}

LIBMTP_file_t *MobileDevice::getFileWithName(QString name) {
    LIBMTP_file_t *fileList = LIBMTP_Get_Filelisting_With_Callback(device, NULL, NULL);
    //Iterate for all the next instances in the list of files
    for(LIBMTP_file_t *file = fileList; file; file = file->next) {
        if(strcmp(file->filename, name.toLocal8Bit().data()) == 0) {
            qDebug() << "Found" << file->filename << "ID" << file->item_id << "Folder ID" << file->parent_id;
            return file;
        }
    }

    qWarning() << "No file named" << name << "found! Returning NULL.";
    return NULL;
}

//Static method
QList<MobileDevice *> *MobileDevice::getDevices() {

    LIBMTP_mtpdevice_t *devices;

    switch(LIBMTP_Get_Connected_Devices(&devices)) {
    case LIBMTP_ERROR_NO_DEVICE_ATTACHED:
        qDebug() << "No device attached!";
        return NULL;
    case LIBMTP_ERROR_CONNECTING:
        qDebug() << "MTP connection error!";
        return NULL;
    case LIBMTP_ERROR_MEMORY_ALLOCATION:
        qDebug() << "MTP memory allocation error!";
        return NULL;
    case LIBMTP_ERROR_PTP_LAYER:
        qDebug() << "PTP layer error!";

        return NULL;
    case LIBMTP_ERROR_USB_LAYER:
        qDebug() << "USB layer error!";
        return NULL;
    case LIBMTP_ERROR_STORAGE_FULL:
        qDebug() << "Storage full error!";
        return NULL;
    case LIBMTP_ERROR_CANCELLED:
        qDebug() << "Operation canceled!";
        return NULL;
    case LIBMTP_ERROR_GENERAL:
        qDebug() << "General error with libmtp!";
        return NULL;

    case LIBMTP_ERROR_NONE:
        break;
    }

    QList<MobileDevice *> *deviceList = new QList<MobileDevice *>();
    //Iterate through the devices and add each to the list
    for(LIBMTP_mtpdevice_t *iterator = devices; iterator; iterator = iterator->next) {
        LIBMTP_Dump_Errorstack(iterator);
        LIBMTP_Clear_Errorstack(iterator);

        deviceList->append(new MobileDevice(iterator));
    }

    return deviceList;
}

QString MobileDevice::toString() {
    return QString(LIBMTP_Get_Modelname(device));
}
