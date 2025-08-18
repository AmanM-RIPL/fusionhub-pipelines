#include "repositories/task_measurement_repository.h"
TaskMeasurementRepository::TaskMeasurementRepository() {}
std::unique_ptr<TaskMeasurement> TaskMeasurementRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<TaskMeasurement>> TaskMeasurementRepository::findAll() { return {}; }
bool TaskMeasurementRepository::save(const TaskMeasurement& entity) { return false; }
bool TaskMeasurementRepository::update(const TaskMeasurement& entity) { return false; }
bool TaskMeasurementRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<TaskMeasurement>> TaskMeasurementRepository::findByApprovalStatus(bool status) { return {}; }
QString TaskMeasurementRepository::getTableName() const { return "TaskMeasurement"; }
std::unique_ptr<TaskMeasurement> TaskMeasurementRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void TaskMeasurementRepository::bindEntityToQuery(QSqlQuery& query, const TaskMeasurement& entity) const {}
QString TaskMeasurementRepository::getInsertQuery() const { return ""; }
QString TaskMeasurementRepository::getUpdateQuery() const { return ""; }
