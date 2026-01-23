#ifndef BACKGROUND_THREAD_MANAGER_H
#define BACKGROUND_THREAD_MANAGER_H

#include <QObject>
#include <QtConcurrent>

class BackgroundThreadManager : public QObject
{
    Q_OBJECT
public:
    explicit BackgroundThreadManager(QObject *parent = nullptr);

    static BackgroundThreadManager* instance()
    {
        static BackgroundThreadManager _instance;
        return &_instance;
    }

// signals:
//     void runBackgroundTaskForDraftDataSync();

public slots:
    void runBackgroundTaskForDraftDataSync();
};

#endif // BACKGROUND_THREAD_MANAGER_H
