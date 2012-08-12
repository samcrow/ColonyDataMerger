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
#include <colony.hpp>

#include "libraries/include/qjson/parser.h"
#include "colonydatamerger.hpp"


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
      A file named colonies.csv must be present on the device in the correct folder.
      The existing file on the device will be deleted and replaced with the new one.
      Do not call close() on the file after it is uploaded with this method.
      @param file The file on the local filesystem to upload
      */
    void uploadCSV(QFile *file);

    /**
      Upload CSV data to the device. The text will be written to a file on the device.
      @param text The text to upload and write
      */
    void uploadCSV(QString text);



    /**
      Read the JSON file from the device and return (a pointer to) the JSON-formatted text
      that it contains. If the file is not there or could not be read, this method
      returns NULL.
      */
    QString getJsonText();

    /**
      Read the JSON file from the device and parse it into memory.
      This will return a map with "colonies" mapped to a QVariantList of colonies.
      */
    QVariantMap getJson();

    /**
      Read the JSON file from the device and parse it into a newly allocated list of newly allocated colonies.
      */
    QList<Colony *> *getColonies();

    QString toString();

signals:

public slots:

protected:

    LIBMTP_mtpdevice_t *device;

    //Protected constructor to create a Device from a libmtp mtpdevice_t
    MobileDevice(LIBMTP_mtpdevice_t *inDevice);

    /**
      Get the reference to the CSV file
      */
    LIBMTP_file_t *getCsvFile();

    /**
      Get the reference to the JSON file
      */
    LIBMTP_file_t *getJsonFile();

    /**
      Find a file on the device with a given name
      */
    LIBMTP_file_t *getFileWithName(QString name);

private:


};

#endif // MOBILEDEVICE_HPP
