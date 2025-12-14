#ifndef TASK_CONTROLLER_H
#define TASK_CONTROLLER_H

#include <QObject>
#include "repositories/task_repository.h"
#include "repositories/draft_entity_repository.h"

class TaskController: public QObject
{
    Q_OBJECT
public:
    explicit TaskController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &taskName ,const QString &description, const QString &bimElement ,const QString &startDate ,const QString &endDate, const int pid) const;
    Q_INVOKABLE std::vector<Task*> getTaskList(bool isApproved = false) const;



private:
    TaskRepository* m_taskRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // TASK_CONTROLLER_H
