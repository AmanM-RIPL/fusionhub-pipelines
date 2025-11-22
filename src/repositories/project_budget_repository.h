#ifndef PROJECT_BUDGET_REPOSITORY_H
#define PROJECT_BUDGET_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/project_budget.h"
#include <memory>
class ProjectBudgetRepository : public QObject, public AbstractRepository<ProjectBudget> {
    Q_OBJECT
public:
    explicit ProjectBudgetRepository(QObject* parent = nullptr);
    Q_INVOKABLE std::unique_ptr<ProjectBudget> findById(int id) override;
    std::vector<std::unique_ptr<ProjectBudget>> findAll() override;
    Q_INVOKABLE std::vector<ProjectBudget*> findAllQML();
    bool save(const ProjectBudget& entity) override;
    Q_INVOKABLE bool saveQML(ProjectBudget* entity);
    bool update(const ProjectBudget& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ProjectBudget>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<ProjectBudget> mapFromQuery(const QSqlQuery& query) const override;
    ProjectBudget* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const ProjectBudget& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif

