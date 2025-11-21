#include "models/project_budget.h"

ProjectBudget::ProjectBudget(int id, const QString& globalId, bool approvalStatus,
                   const QString& dollarValue,
                   int budgetHeadId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    dollarValue(dollarValue),
    budgetHeadId(budgetHeadId)
{
}
