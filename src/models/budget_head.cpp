#include "models/budget_head.h"

BudgetHead::BudgetHead(int id, const QString& globalId, bool approvalStatus, const QString& description, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus), description(description)
{
}
