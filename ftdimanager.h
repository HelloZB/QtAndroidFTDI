#ifndef FTDIMANAGER_H
#define FTDIMANAGER_H

#include <QObject>
#include <QAndroidJniObject>
#include <QtCore>
#include <QDateTime>
#include <QAndroidJniEnvironment>
#include <QTimer>
#include "qmlitemmodel.h"
#include "customproxy4qmlmodel.h"

class FtdiManager : public QObject
{
    Q_OBJECT
public:
    explicit FtdiManager(QObject *parent = 0);
    QmlItemModel *deviceModel;
    QmlItemModel *historyModel;

    CustomProxy4QmlModel *proxy_historyModel;


signals:
    void addLineToLog(QString);

    void startReadTmr();

    void stopReadTmr();

    void onFtdiStateChanged(bool);

    void onConnectPageSettt(int, int,int ,int,int);

    void onTerminalPageSettt(int,bool);

    void onSettPage(qreal);

    void setCurrntPortIndx(int);

    void sendLineHexIsValid(bool);

public slots:
    void startJar();


    void onBtnPrssd();

    void createDeviceInfoList(bool onlyUpdate = false);

    void deviceStatus();

    void connectToOnePort();

    void connectToOnePort(int indx , int baudIndx, int dataIndx, int stopIndx, int parityIndx, int flowIndx);

    void disconnectFromPort();

    void sendDataToPort(QString line, bool hexOut, int endLineIndx);

    void setThisHistoryFilter(const QString &str);

    void delThisHistoryIndx(const int &indx);

    void saveQmlSett(const int &intVal, const int &key);

    void saveQmlSett(const bool &boolVal, const int &key);


    void saveQmlSettRealVal(const qreal &realVal, const int &key);

    void sendLineHexValidator(QString line);

private slots:
    void onReadTmr();

private:
    bool readData();

    QList<QVariant> loadSettings(int key);
    void saveSettings(QList<QVariant> list, int key);



    int bufLen;

    QAndroidJniObject qtFtDev2xxManager;
    QAndroidJniObject adnrActivity;

    QAndroidJniObject ftD2xx;

    QStringList listHistoryCommand;

};

#endif // FTDIMANAGER_H
