#ifndef MOBILEDEVICE_HPP
#define MOBILEDEVICE_HPP

#include <QObject>
#include <QDebug>
#include <QtGlobal>
#include <QFile>
#include <QTemporaryFile>
#include <QString>
#include <QTextStream>

#include <libmtp.h>


class MobileDevice : public QObject
{
    Q_OBJECT
public:
    explicit MobileDevice(QObject *parent = 0);

    /**
      Get the connected devices. Use this. Don't use the constructor of this class.
      LIBMTP_Init() must be called before this method is used.
      */
    static QList<MobileDevice *> *getDevices();

    /**
      Get the device's current battery level from 0 to 100
      */
    int getBatteryLevel();

    /**
      Upload a CSV file onto the device. The application on the device will use this as the base
      set of colony data. This method should block while the file is being transferred.
      @param file The file on the local filesystem to upload
      */
    void uploadCSV(QFile *file);

    /**
      Upload CSV data to the device. The text will be written to a file on the device.
      @param text The text to upload and write
      */
    void uploadCSV(QString *text);



    /**
      Read the JSON file from the device and return (a pointer to) the JSON-formatted text
      that it contains. If the file is not there or could not be read, this method
      returns NULL.
      */
    QString *getJsonText();


    QString *toString();

signals:

public slots:

protected:

    LIBMTP_mtpdevice_t *device;

    LIBMTP_folder_t *folder;

    //Protected constructor to create a Device from a libmtp mtpdevice_t
    MobileDevice(LIBMTP_mtpdevice_t *inDevice);

    /**
      Traverse the folder structure on the device and get a reference
      to the folder that should contain colony data
      */
    LIBMTP_folder_t *getFolder();

private:


};

#endif // MOBILEDEVICE_HPP
