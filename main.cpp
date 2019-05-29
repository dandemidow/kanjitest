#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "kanjigame.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    KanjiGame kanji;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("kanji", &kanji);
    engine.
    QObject::connect(&kanji, SIGNAL(newItem()), &engine, SLOT(show()));
    QObject::connect(&kanji, SIGNAL(chooseItem()), &engine, SLOT(hide()));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
