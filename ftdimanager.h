#ifndef FTDIMANAGER_H
#define FTDIMANAGER_H

#include <QObject>
#include <QAndroidJniObject>
#include <QtCore>
#include <QDateTime>
#include <QAndroidJniEnvironment>
#include <QTimer>


class FtdiManager : public QObject
{
    Q_OBJECT
public:
    explicit FtdiManager(QAndroidJniObject adnrActivity, QObject *parent = 0);

    void startJar();
signals:
    void addLineToLog(QString);

    void startReadTmr();

    void stopReadTmr();


public slots:
    void onBtnPrssd();

    void createDeviceInfoList();

    void deviceStatus();

    void connectToOnePort();

    void disconnectFromPort();

    void sendDataToPort(QString line);

private slots:
    void onReadTmr();

private:
    bool readData();

    int sendDataToPort(unsigned char *line, int len);


    int bufLen;

    QAndroidJniObject qtFtDev2xxManager;
    QAndroidJniObject adnrActivity;

    QAndroidJniObject ftD2xx;

};

#endif // FTDIMANAGER_H
