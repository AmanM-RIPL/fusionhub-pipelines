#ifndef TASK_MEASUREMENT_REPOSITORY_H
#define TASK_MEASUREMENT_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/task_measurement.h"
#include <memory>
class TaskMeasurementRepository : public AbstractRepository<TaskMeasurement> {
public:
    TaskMeasurementRepository();
    std::unique_ptr<TaskMeasurement> findById(int id) override;
    std::vector<std::unique_ptr<TaskMeasurement>> findAll() override;
    bool save(const TaskMeasurement& entity) override;
    bool update(const TaskMeasurement& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<TaskMeasurement>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<TaskMeasurement> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const TaskMeasurement& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
