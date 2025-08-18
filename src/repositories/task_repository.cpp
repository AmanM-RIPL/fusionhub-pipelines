#include "repositories/task_repository.h"
TaskRepository::TaskRepository() {}
std::unique_ptr<Task> TaskRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<Task>> TaskRepository::findAll() { return {}; }
bool TaskRepository::save(const Task& entity) { return false; }
bool TaskRepository::update(const Task& entity) { return false; }
bool TaskRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<Task>> TaskRepository::findByApprovalStatus(bool status) { return {}; }
QString TaskRepository::getTableName() const { return "Task"; }
std::unique_ptr<Task> TaskRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void TaskRepository::bindEntityToQuery(QSqlQuery& query, const Task& entity) const {}
QString TaskRepository::getInsertQuery() const { return ""; }
QString TaskRepository::getUpdateQuery() const { return ""; }
