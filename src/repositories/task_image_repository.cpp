#include "repositories/task_image_repository.h"
TaskImageRepository::TaskImageRepository() {}
std::unique_ptr<TaskImage> TaskImageRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<TaskImage>> TaskImageRepository::findAll() { return {}; }
bool TaskImageRepository::save(const TaskImage& entity) { return false; }
bool TaskImageRepository::update(const TaskImage& entity) { return false; }
bool TaskImageRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<TaskImage>> TaskImageRepository::findByApprovalStatus(bool status) { return {}; }
QString TaskImageRepository::getTableName() const { return "TaskImage"; }
std::unique_ptr<TaskImage> TaskImageRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void TaskImageRepository::bindEntityToQuery(QSqlQuery& query, const TaskImage& entity) const {}
QString TaskImageRepository::getInsertQuery() const { return ""; }
QString TaskImageRepository::getUpdateQuery() const { return ""; }
