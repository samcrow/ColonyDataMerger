#include "mobiledevice.hpp"

MobileDevice::MobileDevice(QObject *parent) :
    QObject(parent)
{
    qWarning() << "Usage error: Can't construct a MobileDevice independently. Use MobileDevice::getDevices() instead.";
}

MobileDevice::MobileDevice(LIBMTP_mtpdevice_t *inDevice) {
    device = inDevice;
    folder = getFolder();
}

int MobileDevice::getBatteryLevel() {
    uint8_t max = 100;
    uint8_t actual;
    LIBMTP_Get_Batterylevel(device, &max, &actual);
    return actual;
}

void MobileDevice::uploadCSV(QFile *file) {
    if(file->isOpen()) {
        file->close();
    }

    //Try to delete the old file, if it exists


    LIBMTP_file_t *remoteFile = LIBMTP_new_file_t();
    remoteFile->filesize = file->size();

    //Put the file in the colonies folder
    remoteFile->parent_id = folder->folder_id;

    remoteFile->filename = strdup("colonies.csv");
    remoteFile->filetype = LIBMTP_FILETYPE_UNKNOWN;

    qDebug() << "About to send file";
    char *path = file->fileName().toAscii().data();
    qDebug() << "File path" << path;
    int ret = LIBMTP_Send_File_From_File(device, path, remoteFile, NULL, NULL);
    qDebug() << "Send file returned" << ret;
    LIBMTP_Dump_Errorstack(device);
    LIBMTP_Clear_Errorstack(device);

}

void MobileDevice::uploadCSV(QString *text) {

    //Write the text to a temporary file so that libmtp can upload it
    QTemporaryFile file;
    file.open();
    file.write(text->toUtf8());//Write that to the file
    file.flush();//Verify that everything's actually been written
    uploadCSV(&file);
    file.close();

}

QString *MobileDevice::getJsonText() {
    LIBMTP_file_t *remoteFile;
    //TODO

    QString *text = new QString();

    return text;
}

LIBMTP_folder_t *MobileDevice::getFolder() {

    LIBMTP_folder_t *rootFolder = LIBMTP_Get_Folder_List(device);

    //Iterate through every folder in the root directory until next->sibling is NULL
    for(LIBMTP_folder_t *next = rootFolder->sibling; next; next = next->sibling) {
        //If the folder's name matches "Colonies"
        if(strcmp(next->name, "colonies") == 0) {
            qDebug() << "Got folder" << next->folder_id << "named" << next->name;
            return next;
        }
    }
    qWarning() << "No folder named \"colonies\" found. Creating it. This operation may fail.";

    int id = LIBMTP_Create_Folder(device, strdup("colonies"), 0, 0);

    if(id == 0) {
        qWarning() << "Error creating colonies folder!";
        LIBMTP_Dump_Errorstack(device);
        LIBMTP_Clear_Errorstack(device);
        return NULL;
    }
    else {
        qDebug() << "colonies folder with id" << id << "created successfully.";
    }

    return getFolder();
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
