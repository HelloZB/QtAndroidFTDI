#include "ftdimanager.h"
#include <QSettings>
#include <QtAndroid>

//=============================================================================================================================
FtdiManager::FtdiManager(QObject *parent) : QObject(parent)
{
    QTimer *readTmr = new QTimer;
    readTmr->setInterval(150);
    readTmr->setSingleShot(true);

    connect(this, SIGNAL(startReadTmr()), readTmr, SLOT(start()) );
    connect(this, SIGNAL(stopReadTmr()), readTmr, SLOT(stop()) );
    connect(readTmr, SIGNAL(timeout()), this, SLOT(onReadTmr()) );

    deviceModel = new QmlItemModel(QString("value").split(",") );
    historyModel = new QmlItemModel(QString("value").split(",") );

    QList<int> listInt;
    listInt.append(Qt::UserRole + 1);
    proxy_historyModel = new CustomProxy4QmlModel(listInt);
    proxy_historyModel->setDynamicSortFilter(true);
    proxy_historyModel->setSourceModel(historyModel);

    this->adnrActivity = QtAndroid::androidActivity();

}
//=============================================================================================================================
void FtdiManager::startJar()
{
    qtFtDev2xxManager =  QAndroidJniObject(QByteArray("org/qtproject/qt5/QtFtDev").constData());

//    qDebug() << "context is valid = " <<adnrActivity.isValid() << adnrActivity.object() <<  qtFtDev2xxManager.isValid() << qtFtDev2xxManager.object() << QAndroidJniObject::isClassAvailable(QByteArray("org/qtproject/qt5/QtFtDev").constData());

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "1 env =================================================== ";
        env->ExceptionClear();
    }
    qDebug() << "getInstance = " << qtFtDev2xxManager.callMethod<jint>(QByteArray("getInstance").constData(), QByteArray("(Landroid/content/Context;)I").constData(), adnrActivity.object());
//                                                                           QByteArray("(I)Ljava/lang/Object;").constData(), adnrActivity.object());

    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "2 env =================================================== ";
        env->ExceptionClear();
    }

    QTimer::singleShot(200, this, SLOT(createDeviceInfoList()));
    loadSettings(0);

    historyModel->clearAllData();
    for(int i = 0, iMax = listHistoryCommand.size(); i < iMax; i++)
        historyModel->addCustomDevice(QList<QVariant>() << listHistoryCommand.at(i));

}
//=============================================================================================================================
void FtdiManager::onBtnPrssd()
{
    QAndroidJniObject strhello = QAndroidJniObject::callStaticObjectMethod(QByteArray("org/qtproject/qt5/QtFtDev").constData(),
                                                                           QByteArray("fromNumberOne").constData(),
                                                                           QByteArray("(I)Ljava/lang/String;").constData(),
                                                                           QTime::currentTime().second());
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "4env =================================================== ";
        env->ExceptionClear();
    }

    //        jstring strJ = strhello.object<jstring>();

    qDebug() << "stringNumber=" << strhello.toString(); // <<strJ;
    emit addLineToLog(QDateTime::currentDateTime().toString());
    connectToOnePort();
}
//=============================================================================================================================
void FtdiManager::createDeviceInfoList(bool onlyUpdate)
{
    deviceModel->clearAllData();

    jint devCount = qtFtDev2xxManager.callMethod<jint>(QByteArray("createDeviceList").constData());

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    for(int i = 0; i < devCount; i++)
        deviceModel->addCustomDevice(QList<QVariant>() << QString("ttyUSB%1").arg(i));

    if(devCount > 0){
        emit setCurrntPortIndx(devCount - 1);
    }else
        emit setCurrntPortIndx(-1);
    if(devCount > 0 && !onlyUpdate)
        connectToOnePort();
}
//=============================================================================================================================
void FtdiManager::deviceStatus()
{
    jint devCount = qtFtDev2xxManager.callMethod<jint>(QByteArray("checkDeviceStatus").constData());

    QString devStts("");
    switch(devCount){
    case 0: devStts = "DEV_NOT_CONNECT"; break;
    case 1: devStts = "DEV_NOT_CONFIG"; break;
    case 2: devStts = "DEV_CONFIG"; break;
    default: devStts = "unknown"; break;
    }

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }
    emit addLineToLog("devStts= " + devStts + " retVal=" + QString::number(devCount));
}
//=============================================================================================================================
void FtdiManager::connectToOnePort()
{
    QList<QVariant> list = loadSettings(1);
    while(list.size() < 4)
        list.append(0);

    if(list.size() < 5)
        list.append(3);

    connectToOnePort(0, list.at(4).toInt(), list.at(0).toInt(), list.at(1).toInt(), list.at(2).toInt(), list.at(3).toInt());
}
//=============================================================================================================================
void FtdiManager::connectToOnePort(int indx, int baudIndx, int dataIndx, int stopIndx, int parityIndx, int flowIndx)
{

    QList<QVariant> listVar;
    listVar.append(dataIndx);
    listVar.append(stopIndx);
    listVar.append(parityIndx);
    listVar.append(flowIndx);
    listVar.append(baudIndx);
    saveSettings(listVar, 1);

    jint baudRate ;

    switch (baudIndx) {
    case 0: baudRate = 1200; break;
    case 1: baudRate = 2400; break;
    case 2: baudRate = 4800; break;
    case 3: baudRate = 9600; break;
    case 4: baudRate = 19200; break;
    case 5: baudRate = 38400; break;
    case 6: baudRate = 57600; break;
    case 7: baudRate = 115200; break;

    default: baudRate = 9600; break;
    }

    jint devIndx = indx;
    jint devDataIndx = dataIndx;
    jint devStopIndx = stopIndx ;
    jint devParityIndx = parityIndx;
    jint devFlowIndx = flowIndx;

    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("connToOneDevice").constData(),
                                                     QByteArray("(IIIIII)I").constData(), devIndx,
                                                     baudRate,
                                                     devDataIndx,
                                                     devStopIndx,
                                                     devParityIndx,
                                                     devFlowIndx  );

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    QString openStts("");
    switch (retVal) {
    case 0: openStts = tr("Opened ttyUSB%1!").arg(indx) ;  emit onFtdiStateChanged(true); emit startReadTmr(); break;
    case 1: openStts = tr("Already opened ttyUSB%1!").arg(indx);  emit onFtdiStateChanged(true); emit startReadTmr(); break;
    case 2: openStts = tr("Device not valid ttyUSB%1!").arg(indx); break;
    case 3: openStts = tr("Can't open ttyUSB%1!").arg(indx); break;

    default: openStts = "unkonown error";   break;
    }

    emit addLineToLog(openStts );
}
//=============================================================================================================================
void FtdiManager::disconnectFromPort()
{
    emit stopReadTmr();
    emit onFtdiStateChanged(false);
    emit addLineToLog("Device disconected");
    qtFtDev2xxManager.callMethod<jint>(QByteArray("disconnectFunction").constData());

    createDeviceInfoList(true);
}
//=============================================================================================================================
void FtdiManager::sendDataToPort(QString line, bool hexOut, int endLineIndx)
{
    QString tmpStr(line);
    emit startReadTmr();

    if(hexOut){
        switch(endLineIndx){
        case 0:  line.append("0D0A"); break;
        case 1:  line.append("0D"); break;
        case 2:  line.append("0A"); break;
        default: break;
        }
    }else{

        switch(endLineIndx){
        case 0:  line.append("\r\n"); break;
        case 1:  line.append("\r"); break;
        case 2:  line.append("\n"); break;
        default: break;
        }
    }


    QAndroidJniEnvironment env;
    if(!hexOut)
        line = line.toLocal8Bit().toHex();

    if((line.size() % 2))
        line.prepend("0");

    QAndroidJniObject string2 = QAndroidJniObject::fromString(line);

    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("sendData").constData(),
                                                     QByteArray("(Ljava/lang/String;)I").constData(),
                                                     string2.object<jstring>()/*, arr*/);
    if (env->ExceptionCheck()) {
        // Handle exception here.
        emit addLineToLog("write exeption");
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    if(!listHistoryCommand.contains(tmpStr.simplified().trimmed(), Qt::CaseInsensitive)){
        listHistoryCommand.prepend(tmpStr.simplified().trimmed());
        historyModel->prependCustomDevice(QList<QVariant>() << listHistoryCommand.first());
        saveSettings(QList<QVariant>(), 4); //save list last command
    }

    if(retVal >= 0){
        emit addLineToLog(tmpStr);
    }else{
        emit addLineToLog("line error write\r\n" + QString::number(retVal));
        disconnectFromPort();
    }
}
//=============================================================================================================================
void FtdiManager::setThisHistoryFilter(const QString &str)
{
    proxy_historyModel->setNewFileterStr(str);
}
//=============================================================================================================================
void FtdiManager::delThisHistoryIndx(const int &indx)
{
    int row = proxy_historyModel->mapToSource(proxy_historyModel->index(indx,0)).row();
    if(row < 0 || row > listHistoryCommand.size())
        return;
    historyModel->removeThisRow(row);
    listHistoryCommand.removeAt(row);
    saveSettings(QList<QVariant>(), 4);
}
//=============================================================================================================================
void FtdiManager::saveQmlSett(const int &intVal, const int &key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");

    switch(key){
    case 0: {  settings.beginGroup("terminal");   settings.setValue("endSymb", intVal);  break;}
    }
    settings.endGroup();
}
//=============================================================================================================================
void FtdiManager::saveQmlSett(const bool &boolVal, const int &key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");

    switch(key){
    case 0: {  settings.beginGroup("terminal");    settings.setValue("hexOut", boolVal);  break;}

    }
    settings.endGroup();

}
//=============================================================================================================================
void FtdiManager::saveQmlSettRealVal(const qreal &realVal, const int &key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");

    switch(key){
    case 0: {  settings.beginGroup("sett");    settings.setValue("fontSize", realVal);  break;}

    }
    settings.endGroup();

}
//=============================================================================================================================
void FtdiManager::sendLineHexValidator( QString line)
{
    line = line.simplified().trimmed().remove(" ");
    bool isValid = false;
    if(line.isEmpty()){
        isValid = true;
    }else{
        if(line.size()%2){
            isValid = false;
        }else{

            QString tmpStr = QByteArray::fromHex(line.toLocal8Bit()).toHex();
            isValid = (tmpStr.length() == line.length());
        }
    }

    emit sendLineHexIsValid(isValid);
}
//=============================================================================================================================
void FtdiManager::onReadTmr()
{
    if(readData())
        emit startReadTmr();
    else{
        disconnectFromPort();

    }
}
//=============================================================================================================================
bool FtdiManager::readData()
{
    jint bytesAvailable = qtFtDev2xxManager.callMethod<jint>(QByteArray("bytesAvailable").constData());
    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        emit addLineToLog("read errror");

        env->ExceptionClear();
        return false;
    }

    if(bytesAvailable < 0){
        return false;
    }

    if(bytesAvailable < 1)
        return true;

    QAndroidJniObject stringArray = qtFtDev2xxManager.callObjectMethod<jstring>(QByteArray("readData").constData());
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        emit addLineToLog("read errror");

        env->ExceptionClear();
        disconnectFromPort();
        return false;
    }
    QString readStr = stringArray.toString();
    if(readStr.isEmpty())
        return true;

    if((readStr.length()%2))
        readStr.prepend("0");

    readStr = QByteArray::fromHex(readStr.toLocal8Bit());

    if(!readStr.isEmpty())
        emit addLineToLog(readStr);

    return true;
}
//=============================================================================================================================
QList<QVariant> FtdiManager::loadSettings(int key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");
    QList<QVariant> list;
    switch(key){
    case 0: { //load all settings
        settings.beginGroup("uart");
        emit onConnectPageSettt(settings.value("dataBits", 0).toInt(),
                                settings.value("stopBits", 0).toInt(),
                                settings.value("parity", 0).toInt(),
                                settings.value("flowCtrl", 0).toInt(),
                                settings.value("baudRate", 3).toInt() );

        settings.endGroup();

        settings.beginGroup("terminal");
        emit onTerminalPageSettt(settings.value("endSymb", 0).toInt(), settings.value("hexOut", false).toBool());
        settings.endGroup();

        settings.beginGroup("sett");
        emit onSettPage(settings.value("fontSize", 0).toReal());
        settings.endGroup();

        settings.beginGroup("app");
        listHistoryCommand = settings.value("listHistoryCommand").toStringList();
        if(listHistoryCommand.isEmpty())
            listHistoryCommand = QString("+++ ATAD ATBD ATCH ATID ATNI ATSL ATSH ATVR ATHV ATAP ATSM ATND ATBD").split(' ');
        settings.endGroup();

        return list; }

    case 1:{
        settings.beginGroup("uart");
        list.append(settings.value("dataBits", 0));
        list.append(settings.value("stopBits", 0));
        list.append(settings.value("parity", 0));
        list.append(settings.value("flowCtrl", 0));
        list.append(settings.value("baudRate", 3));

        settings.endGroup();
        return list;}
    }
    return list;
}
//=============================================================================================================================
void FtdiManager::saveSettings(QList<QVariant> list, int key)
{
    QSettings settings("Hello_ZB", "QtAndroidFtdiTerminal");
    switch(key){
    case 0: { //save all settings
        settings.beginGroup("uart");
        if(list.size() != 8)
            return;
        settings.setValue("dataBits", list.at(0));
        settings.setValue("stopBits", list.at(1));
        settings.setValue("parity", list.at(2));
        settings.setValue("flowCtrl", list.at(3));
        settings.setValue("baudRate", list.at(4));
        settings.endGroup();

        settings.beginGroup("terminal");
        settings.setValue("endSymb", list.at(5));
        settings.setValue("hexOut", list.at(6));
        settings.endGroup();

        settings.beginGroup("sett");
        settings.setValue("fontSize", list.at(7));
        settings.endGroup();

        while(listHistoryCommand.size() > 1000)
            listHistoryCommand.removeLast();

        settings.beginGroup("app");
        settings.setValue("listHistoryCommand",listHistoryCommand);
        settings.endGroup();

        return ; }

    case 1:{
        settings.beginGroup("uart");
        if(list.size() != 5)
            return;
        settings.setValue("dataBits", list.at(0));
        settings.setValue("stopBits", list.at(1));
        settings.setValue("parity", list.at(2));
        settings.setValue("flowCtrl", list.at(3));
        settings.setValue("baudRate", list.at(4));

        settings.endGroup();
        return ;}

    case 2:{
        if(list.size() != 2)
            return;
        settings.beginGroup("terminal");
        settings.setValue("endSymb", list.at(0));
        settings.setValue("hexOut", list.at(1));
        settings.endGroup();
        return; }

    case 3:{
        if(list.size() != 1)
            return;
        settings.beginGroup("sett");
        settings.setValue("fontSize", list.at(0));
        settings.endGroup();
        return; }

    case 4:{
        while(listHistoryCommand.size() > 1000)
            listHistoryCommand.removeLast();

        settings.beginGroup("app");
        settings.setValue("listHistoryCommand",listHistoryCommand);
        settings.endGroup();
        return;}
    }
}
//=============================================================================================================================
