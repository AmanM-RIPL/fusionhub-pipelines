#include "repositories/project_budget_repository.h"
ProjectBudgetRepository::ProjectBudgetRepository() {}
std::unique_ptr<ProjectBudget> ProjectBudgetRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ProjectBudget>> ProjectBudgetRepository::findAll() { return {}; }
bool ProjectBudgetRepository::save(const ProjectBudget& entity) { return false; }
bool ProjectBudgetRepository::update(const ProjectBudget& entity) { return false; }
bool ProjectBudgetRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ProjectBudget>> ProjectBudgetRepository::findByApprovalStatus(bool status) { return {}; }
QString ProjectBudgetRepository::getTableName() const { return "ProjectBudget"; }
std::unique_ptr<ProjectBudget> ProjectBudgetRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void ProjectBudgetRepository::bindEntityToQuery(QSqlQuery& query, const ProjectBudget& entity) const {}
QString ProjectBudgetRepository::getInsertQuery() const { return ""; }
QString ProjectBudgetRepository::getUpdateQuery() const { return ""; }
