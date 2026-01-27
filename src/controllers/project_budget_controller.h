#ifndef PROJECT_BUDGET_CONTROLLER_H
#define PROJECT_BUDGET_CONTROLLER_H

#include <QObject>
#include "repositories/project_budget_repository.h"
#include "repositories/draft_entity_repository.h"

class ProjectBudgetController: public QObject
{
    Q_OBJECT
public:
    explicit ProjectBudgetController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &name, const int &BudgetHeadId) const;

    Q_INVOKABLE void update(int id, const QString &name, const int &BudgetHeadId) const;

    Q_INVOKABLE std::vector<ProjectBudget*> getProjectBudgetList(bool isApproved = false) const;

private:
    ProjectBudgetRepository* m_projectBudgetRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // PROJECT_BUDGET_CONTROLLER_H
