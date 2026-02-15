#include "repositories/project_budget_repository.h"

ProjectBudgetRepository::ProjectBudgetRepository(QObject* parent) : QObject(parent) {}
std::unique_ptr<ProjectBudget> ProjectBudgetRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ProjectBudget>> ProjectBudgetRepository::findAll() { return {}; }
std::vector<ProjectBudget*> ProjectBudgetRepository::findAllQML() {
    std::vector<ProjectBudget*> budgetHead;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM ProjectBudget")) {
        while (query.next()) {
            budgetHead.push_back(mapFromQueryQML(query, this));
        }
    }

    return budgetHead;
}
bool ProjectBudgetRepository::save(const ProjectBudget& entity) { return false; }
bool ProjectBudgetRepository::saveQML(ProjectBudget* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool ProjectBudgetRepository::update(const ProjectBudget& entity) { return false; }
bool ProjectBudgetRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ProjectBudget>> ProjectBudgetRepository::findByApprovalStatus(bool status) { return {}; }
QString ProjectBudgetRepository::getTableName() const { return "ProjectBudget"; }
std::unique_ptr<ProjectBudget> ProjectBudgetRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

ProjectBudget* ProjectBudgetRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto projectBudget = new ProjectBudget(parent);
    projectBudget->setId(query.value("id").toInt());
    projectBudget->setGlobalId(query.value("global_id").toString());
    projectBudget->setApprovalStatus(query.value("approval_status").toString());
    projectBudget->setDollarValue(query.value("dollar_value").toString());
    projectBudget->setBudgetHeadId(query.value("budget_head_id").toInt());

    return projectBudget;
}
void ProjectBudgetRepository::bindEntityToQuery(QSqlQuery& query, const ProjectBudget& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getDollarValue());
    query.addBindValue(entity.getBudgetHeadId());
}
QString ProjectBudgetRepository::getInsertQuery() const {
    return "INSERT INTO ProjectBudget (global_id, approval_status, dollar_value, "
           "budget_head_id) "
           "VALUES (?, ?, ?, ?)";
}
QString ProjectBudgetRepository::getUpdateQuery() const {
    return "UPDATE ProjectBudget SET global_id = ?, approval_status = ?, dollar_value = ?, "
           " budget_head_id = ? WHERE id = ?";
}

