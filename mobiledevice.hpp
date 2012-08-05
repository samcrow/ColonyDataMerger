#ifndef MOBILEDEVICE_HPP
#define MOBILEDEVICE_HPP

#include <QObject>
#include <QDebug>
#include <QtGlobal>

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

    QString toString();

signals:

public slots:

protected:

    LIBMTP_mtpdevice_t *device;

    //Protected constructor to create a Device from a libmtp mtpdevice_t
    explicit MobileDevice(LIBMTP_mtpdevice_t *inDevice);

private:


};

#endif // MOBILEDEVICE_HPP
