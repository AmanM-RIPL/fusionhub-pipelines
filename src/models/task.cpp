#include "models/task.h"

Task::Task(int id, const QString& globalId, bool approvalStatus,
           const QString& taskName, const QString& description,
           const QString& bimElement, const QDate& startDate, const QDate& endDate)
    : id(id), globalId(globalId), approvalStatus(approvalStatus),
      taskName(taskName), description(description), bimElement(bimElement),
      startDate(startDate), endDate(endDate)
{
}
