#ifndef TASK_REPOSITORY_H
#define TASK_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/task.h"
#include <memory>
class TaskRepository :public QObject, public AbstractRepository<Task> {
    Q_OBJECT
public:
    explicit TaskRepository(QObject* parent = nullptr);
    Q_INVOKABLE std::unique_ptr<Task> findById(int id) override;
    std::vector<std::unique_ptr<Task>> findAll() override;
    Q_INVOKABLE std::vector<Task*> findAllQML();
    bool save(const Task& entity) override;
    Q_INVOKABLE bool saveQML(Task* entity);
    bool update(const Task& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<Task>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    std::unique_ptr<Task> mapFromQuery(const QSqlQuery& query) const override;
    Task* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const Task& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
