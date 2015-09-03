#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMessageBox>
#include <QDebug>
#include <QtAndroid>
#include "ftdimanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    FtdiManager ftdiManager(QtAndroid::androidActivity());

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("ftdiManager", &ftdiManager);
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


        ftdiManager.startJar();
        return app.exec();
    }else{
        QMessageBox::critical(0,"AndroidFtdi", "Can't load qml content. Check your installation dir.");
        return 3;
    }


}
