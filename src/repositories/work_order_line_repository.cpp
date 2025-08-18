#include "repositories/work_order_line_repository.h"
WorkOrderLineRepository::WorkOrderLineRepository() {}
std::unique_ptr<WorkOrderLine> WorkOrderLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkOrderLine>> WorkOrderLineRepository::findAll() { return {}; }
bool WorkOrderLineRepository::save(const WorkOrderLine& entity) { return false; }
bool WorkOrderLineRepository::update(const WorkOrderLine& entity) { return false; }
bool WorkOrderLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkOrderLine>> WorkOrderLineRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkOrderLineRepository::getTableName() const { return "WorkOrderLine"; }
std::unique_ptr<WorkOrderLine> WorkOrderLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void WorkOrderLineRepository::bindEntityToQuery(QSqlQuery& query, const WorkOrderLine& entity) const {}
QString WorkOrderLineRepository::getInsertQuery() const { return ""; }
QString WorkOrderLineRepository::getUpdateQuery() const { return ""; }
