#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "ArchiveModel.h"
#include "playarchive/FormatManager.h"
#include "playarchive/GCFEngine.h"
#include "playarchive/LibArchiveHandler.h"

using namespace PlayArchive;

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName("PlayArchive");
    app.setApplicationName("PlayArchive");

    // Register handlers
    auto& fm = FormatManager::instance();
    fm.registerHandler("gcf", std::make_shared<GCFEngine>());
    fm.registerHandler("zip", std::make_shared<LibArchiveHandler>("zip"));
    fm.registerHandler("7z", std::make_shared<LibArchiveHandler>("7z"));
    fm.registerHandler("tar", std::make_shared<LibArchiveHandler>("tar"));

    QQmlApplicationEngine engine;
    
    ArchiveModel archiveModel;
    engine.rootContext()->setContextProperty("archiveModel", &archiveModel);

    const QUrl url(QStringLiteral("qrc:/qt/qml/PlayArchive/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
