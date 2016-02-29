#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMessageBox>
#include <QDebug>
#include "ftdimanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FtdiManager ftdiManager;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ftdiManager", &ftdiManager);
    engine.rootContext()->setContextProperty("deviceModel", ftdiManager.deviceModel);
    engine.rootContext()->setContextProperty("historyModel", ftdiManager.proxy_historyModel);

//QtAndroid::androidActivity().callMethod()
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if(!engine.rootObjects().isEmpty()){

        qDebug() << "one line";
        QObject *item = engine.rootObjects().first();

        if(item)
            qDebug() << "item done";
        else
            return 100;

        QObject::connect(&ftdiManager, SIGNAL(addLineToLog(QString)), item, SIGNAL(addToLog(QString)));
        QObject::connect(&ftdiManager, SIGNAL(onFtdiStateChanged(bool)), item, SIGNAL(connStateChanged(bool)));
        QObject::connect(&ftdiManager, SIGNAL(onConnectPageSettt(int,int, int,int ,int)), item, SIGNAL(connectPageSettt(int, int,int ,int, int)));
        QObject::connect(&ftdiManager, SIGNAL(onTerminalPageSettt(int,bool)), item, SIGNAL(terminalPageSettt(int,bool)));
        QObject::connect(&ftdiManager, SIGNAL(onSettPage(qreal)), item, SIGNAL(settPage(qreal)));
        QObject::connect(&ftdiManager, SIGNAL(setCurrntPortIndx(int)), item, SIGNAL(setCurrntPortIndx(int)));
        QObject::connect(&ftdiManager, SIGNAL(sendLineHexIsValid(bool)), item, SIGNAL( sendLineHexIsValid(bool)));

        ftdiManager.startJar();
        return app.exec();
    }else{
        QMessageBox::critical(0,"AndroidFtdi", "Can't load qml content. Check your installation dir.");
        return 3;
    }


}
