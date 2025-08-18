#ifndef TASK_REPOSITORY_H
#define TASK_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/task.h"
#include <memory>
class TaskRepository : public AbstractRepository<Task> {
public:
    TaskRepository();
    std::unique_ptr<Task> findById(int id) override;
    std::vector<std::unique_ptr<Task>> findAll() override;
    bool save(const Task& entity) override;
    bool update(const Task& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<Task>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<Task> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const Task& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
