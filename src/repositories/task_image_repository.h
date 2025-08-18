#ifndef TASK_IMAGE_REPOSITORY_H
#define TASK_IMAGE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/task_image.h"
#include <memory>
class TaskImageRepository : public AbstractRepository<TaskImage> {
public:
    TaskImageRepository();
    std::unique_ptr<TaskImage> findById(int id) override;
    std::vector<std::unique_ptr<TaskImage>> findAll() override;
    bool save(const TaskImage& entity) override;
    bool update(const TaskImage& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<TaskImage>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<TaskImage> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const TaskImage& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
