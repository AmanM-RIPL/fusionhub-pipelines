#include "repositories/work_billing_line_repository.h"
WorkBillingLineRepository::WorkBillingLineRepository() {}
std::unique_ptr<WorkBillingLine> WorkBillingLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkBillingLine>> WorkBillingLineRepository::findAll() { return {}; }
bool WorkBillingLineRepository::save(const WorkBillingLine& entity) { return false; }
bool WorkBillingLineRepository::update(const WorkBillingLine& entity) { return false; }
bool WorkBillingLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkBillingLine>> WorkBillingLineRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkBillingLineRepository::getTableName() const { return "WorkBillingLine"; }
std::unique_ptr<WorkBillingLine> WorkBillingLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void WorkBillingLineRepository::bindEntityToQuery(QSqlQuery& query, const WorkBillingLine& entity) const {}
QString WorkBillingLineRepository::getInsertQuery() const { return ""; }
QString WorkBillingLineRepository::getUpdateQuery() const { return ""; }
