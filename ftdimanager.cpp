#include "ftdimanager.h"
//=============================================================================================================================
FtdiManager::FtdiManager(QAndroidJniObject adnrActivity, QObject *parent) : QObject(parent), adnrActivity(adnrActivity)
{
    QTimer *readTmr = new QTimer;
    readTmr->setInterval(50);
    readTmr->setSingleShot(true);

    connect(this, SIGNAL(startReadTmr()), readTmr, SLOT(start()) );
    connect(this, SIGNAL(stopReadTmr()), readTmr, SLOT(stop()) );
    connect(readTmr, SIGNAL(timeout()), this, SLOT(onReadTmr()) );
}
//=============================================================================================================================
void FtdiManager::startJar()
{


    qtFtDev2xxManager =  QAndroidJniObject(QByteArray("org/qtproject/qt5/QtFtDev").constData());


    qDebug() << "context is valid = " <<adnrActivity.isValid() << adnrActivity.object() <<  qtFtDev2xxManager.isValid() << qtFtDev2xxManager.object() << QAndroidJniObject::isClassAvailable(QByteArray("org/qtproject/qt5/QtFtDev").constData());


    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "1 env =================================================== ";
        env->ExceptionClear();
    }




    qDebug() << "from str 2 = " << qtFtDev2xxManager.callObjectMethod(QByteArray("fromNumberTwo").constData(), QByteArray("(I)Ljava/lang/String;").constData(), 7).toString();

    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "2 env =================================================== ";
        env->ExceptionClear();
    }


    qDebug() << "getInstance = " << qtFtDev2xxManager.callMethod<jint>(QByteArray("getInstance").constData(), QByteArray("(Landroid/content/Context;)I").constData(), adnrActivity.object());
//                                                                       QByteArray("(I)Ljava/lang/Object;").constData(), adnrActivity.object());

    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "2 env =================================================== ";
        env->ExceptionClear();
    }


//    ftD2xx = QAndroidJniObject::callStaticObjectMethod(QByteArray("com/ftdi/j2xx/D2xxManager").constData(),
//                                                       QByteArray("getInstance").constData(),
//                                                       QByteArray("(Landroid/content/Context;)Lcom/ftdi/j2xx/D2xxManager;").constData(),
//                                                       adnrActivity.object());


////                ); ///QAndroidJniObject(QByteArray("com/ftdi/j2xx/D2xxManager").constData());
//    qDebug() << "context is valid = " <<adnrActivity.isValid() << adnrActivity.object() <<  ftD2xx.isValid() << ftD2xx.object() << QAndroidJniObject::isClassAvailable(QByteArray("com/ftdi/j2xx/D2xxManager").constData());


////    QAndroidJniEnvironment env;
//    if (env->ExceptionCheck()) {
//        // Handle exception here.
//        qDebug() << "1 env =================================================== ";
//        env->ExceptionClear();
//    }


    createDeviceInfoList();
//    connectToOnePort();
//    deviceStatus();



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
void FtdiManager::createDeviceInfoList()
{
//    jint devCount = ftD2xx.callMethod<jint>(QByteArray("createDeviceInfoList").constData(),
//                                            QByteArray("(Landroid/content/Context;I)").constData(),
//                                            adnrActivity.object());
    jint devCount = qtFtDev2xxManager.callMethod<jint>(QByteArray("createDeviceList").constData());
//                                                                       QByteArray("(I)Ljava/lang/Object;").constData(), adnrActivity.object());



    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    qDebug() << "devCOund " << devCount;

    emit addLineToLog("devCount= " + QString::number(devCount) );

    if(devCount > 0)
        connectToOnePort();
}
//=============================================================================================================================
void FtdiManager::deviceStatus()
{
    jint devCount = qtFtDev2xxManager.callMethod<jint>(QByteArray("checkDeviceStatus").constData());
//                                                                       QByteArray("(I)Ljava/lang/Object;").constData(), adnrActivity.object());


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

    qDebug() << "devCOund " << devCount;

    emit addLineToLog("devStts= " + devStts + " retVal=" + QString::number(devCount));
}
//=============================================================================================================================
void FtdiManager::connectToOnePort()
{
    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("connToOneDevice").constData(),
                                                                       QByteArray("(I)I").constData(), 9600);

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }

    qDebug() << "retVal " << retVal;

    QString openStts("");
    switch (retVal) {
    case 0: openStts = "Opened!" ; break;
    case 1: openStts = "if( currentPortIndex == portIndex && ftDev != null && true == ftDev.isOpen() )"; break;
    case 2: openStts = "if(ftDev == null)"; break;
    case 3: openStts = "! if (true == ftDev.isOpen()){"; break;

    default: openStts = "unkonown error";   break;
    }



    emit addLineToLog("Open status: " + openStts + " retVal=" + QString::number(retVal) );
}
//=============================================================================================================================
void FtdiManager::disconnectFromPort()
{

}
//=============================================================================================================================
void FtdiManager::sendDataToPort(QString line)
{
    line.append("\r\n");

    QAndroidJniEnvironment env;

    int len = line.size();
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
    emit addLineToLog(QString("SendLine %1 %2").arg(len).arg(retVal));

    qDebug() << "sendLine " << line << len << retVal;



    if(retVal >= 0){
        emit addLineToLog(line);
        emit startReadTmr();
    }else{
        emit addLineToLog("line error write\r\n" + QString::number(retVal));

        qDebug() << "stopReadTmr";
        stopReadTmr();
    }
}
//=============================================================================================================================
void FtdiManager::onReadTmr()
{
    if(readData())
        emit startReadTmr();
}
//=============================================================================================================================
bool FtdiManager::readData()
{
//    int USB_DATA_BUFFER = 8192;
//    jbyteArray arr = qtFtDev2xxManager.callMethod<jbyteArray>(QByteArray("readData").constData());
    //                                                        QByteArray("(I[B)I").constData(), line.size(), arr);

    QAndroidJniObject stringArray = qtFtDev2xxManager.callObjectMethod<jstring>(QByteArray("readData").constData());

    QAndroidJniEnvironment env;

    if (env->ExceptionCheck()) {
        // Handle exception here.
        qDebug() << "11 env =================================================== ";
        emit addLineToLog("read errror");

        env->ExceptionClear();
        return false;
    }
    QString readStr = stringArray.toString();
    if((readStr.length()%2))
        readStr.prepend("0");

    readStr = QByteArray::fromHex(readStr.toLocal8Bit());

//    int len = env->GetArrayLength (arr);

//    if(len < 1)
//        return true;

//    unsigned char* buf = new unsigned char[len];
//    env->GetByteArrayRegion (arr, 0, len, reinterpret_cast<jbyte*>(buf));

////    QByteArray readStr(/*(unsigned char*)*/buf,len);
//    QString readStr("");
//    for(int i = 0; i < len; i++)
//        readStr.append(buf[i]);

//    qDebug() << "readLine " << buf << len << arr;
//    qDebug() << "sendLine " << readStr;

    if(!readStr.isEmpty())
        emit addLineToLog(QString(readStr.toLocal8Bit().toHex()) + "\r\n" + readStr);

    return true;
}
//=============================================================================================================================
int FtdiManager::sendDataToPort(unsigned char *line, int len)
{
    jint lineSize = len;

    QAndroidJniEnvironment env;

//    jbyteArray arr = env->NewByteArray(lineSize);
//    env->SetByteArrayRegion(arr, 0, lineSize,reinterpret_cast<jbyte*>(line) );

    jint retVal = qtFtDev2xxManager.callMethod<jint>(QByteArray("sendData").constData(),
                                                                       QByteArray("(I)I").constData(), lineSize/*, arr*/);
    if (env->ExceptionCheck()) {
        // Handle exception here.
        emit addLineToLog("write exeption");
        qDebug() << "11 env =================================================== ";
        env->ExceptionClear();
    }
    emit addLineToLog(QString("SendLine %1 %2").arg(len).arg(retVal));

    qDebug() << "sendLine " << line << len << retVal;
    return retVal;
}
//=============================================================================================================================
