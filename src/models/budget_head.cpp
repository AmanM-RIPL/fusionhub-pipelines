#include "models/budget_head.h"

BudgetHead::BudgetHead(int id, const QString& globalId, const QString& approvalStatus, const QString& description,int nextApprovingUser,int createdByUser, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus), description(description), nextApprovingUser(nextApprovingUser), createdByUser(createdByUser)
{
}
