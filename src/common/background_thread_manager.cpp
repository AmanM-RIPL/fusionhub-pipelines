#include "background_thread_manager.h"

BackgroundThreadManager::BackgroundThreadManager(QObject *parent)
    : QObject{parent}
{}

void BackgroundThreadManager::runBackgroundTaskForDraftDataSync() {

    QtConcurrent::run([]() {
        qDebug() << "Background task running every 1 second";

    });
}
