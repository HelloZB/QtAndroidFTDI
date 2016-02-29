#ifndef QTANDROIDFTDIIODEVICE_H
#define QTANDROIDFTDIIODEVICE_H

#include <QObject>
#include <QAndroidJniObject>
#include <QtCore>
#include <QDateTime>
#include <QAndroidJniEnvironment>
#include <QTimer>


class QtAndroidFtdiIoDevice : public QObject
{
    Q_OBJECT
public:
    explicit QtAndroidFtdiIoDevice(QAndroidJniObject adnrActivity, QObject *parent = 0);

signals:

public slots:
};

#endif // QTANDROIDFTDIIODEVICE_H
