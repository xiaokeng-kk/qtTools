#include "SerialPortManager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    // qmlRegisterType<SerialPortManager>("qtTools", 1, 0, "SerialPortManager");

    SerialPortManager serial; // C++ 管理

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("serial", &serial);

    engine.loadFromModule("qtTools", "Main");

    return app.exec();
}
