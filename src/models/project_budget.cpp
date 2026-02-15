#include "models/project_budget.h"

ProjectBudget::ProjectBudget(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                   const QString& dollarValue,
                   int budgetHeadId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    dollarValue(dollarValue),
    budgetHeadId(budgetHeadId)
{
}
