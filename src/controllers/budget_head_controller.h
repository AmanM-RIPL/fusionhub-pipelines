#ifndef BUDGET_HEAD_CONTROLLER_H
#define BUDGET_HEAD_CONTROLLER_H

#include <QObject>
#include "repositories/budget_head_repository.h"
#include "repositories/draft_entity_repository.h"

class BudgetHeadController: public QObject
{
    Q_OBJECT
public:
    explicit BudgetHeadController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &description) const;

    Q_INVOKABLE void update(int id, const QString &description) const;

    Q_INVOKABLE std::vector<BudgetHead*> getBudgetHeadList(bool isApproved = false) const;

private:
    BudgetHeadRepository* m_budgetHeadRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // BUDGET_HEAD_CONTROLLER_H
