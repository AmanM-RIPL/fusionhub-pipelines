#include "repositories/work_billing_repository.h"
WorkBillingRepository::WorkBillingRepository() {}
std::unique_ptr<WorkBilling> WorkBillingRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkBilling>> WorkBillingRepository::findAll() { return {}; }
bool WorkBillingRepository::save(const WorkBilling& entity) { return false; }
bool WorkBillingRepository::update(const WorkBilling& entity) { return false; }
bool WorkBillingRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkBilling>> WorkBillingRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkBillingRepository::getTableName() const { return "WorkBilling"; }
std::unique_ptr<WorkBilling> WorkBillingRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void WorkBillingRepository::bindEntityToQuery(QSqlQuery& query, const WorkBilling& entity) const {}
QString WorkBillingRepository::getInsertQuery() const { return ""; }
QString WorkBillingRepository::getUpdateQuery() const { return ""; }
