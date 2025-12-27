#include "models/task.h"

Task::Task(long long id, const QString& globalId, bool approvalStatus,
                   const QString& taskName, const QString& description,const QString& bimElement,
const QString& startDate,const QString& endDate, long long pid, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    taskName(taskName), description(description), bimElement(bimElement), startDate(startDate),
    endDate(endDate), pid(pid)
{
}
