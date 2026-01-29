#ifndef BACKGROUND_THREAD_MANAGER_H
#define BACKGROUND_THREAD_MANAGER_H

#include <QObject>
#include <QMutex>

class BackgroundThreadManager : public QObject
{
    Q_OBJECT

public:
    static BackgroundThreadManager* instance();

public slots:
    // This will be called by QTimer every second
    void runBackgroundTaskForDraftDataSync();

private:
    explicit BackgroundThreadManager(QObject *parent = nullptr);
    BackgroundThreadManager(const BackgroundThreadManager&) = delete;
    BackgroundThreadManager& operator=(const BackgroundThreadManager&) = delete;

private:
    static BackgroundThreadManager* m_instance;
    static QMutex m_mutex;
};

#endif // BACKGROUND_THREAD_MANAGER_H
