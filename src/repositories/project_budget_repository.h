#ifndef PROJECT_BUDGET_REPOSITORY_H
#define PROJECT_BUDGET_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/project_budget.h"
#include <memory>
class ProjectBudgetRepository : public AbstractRepository<ProjectBudget> {
public:
    ProjectBudgetRepository();
    std::unique_ptr<ProjectBudget> findById(int id) override;
    std::vector<std::unique_ptr<ProjectBudget>> findAll() override;
    bool save(const ProjectBudget& entity) override;
    bool update(const ProjectBudget& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ProjectBudget>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<ProjectBudget> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const ProjectBudget& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
