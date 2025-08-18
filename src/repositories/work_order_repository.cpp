#include "repositories/work_order_repository.h"
WorkOrderRepository::WorkOrderRepository() {}
std::unique_ptr<WorkOrder> WorkOrderRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkOrder>> WorkOrderRepository::findAll() { return {}; }
bool WorkOrderRepository::save(const WorkOrder& entity) { return false; }
bool WorkOrderRepository::update(const WorkOrder& entity) { return false; }
bool WorkOrderRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkOrder>> WorkOrderRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkOrderRepository::getTableName() const { return "WorkOrder"; }
std::unique_ptr<WorkOrder> WorkOrderRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void WorkOrderRepository::bindEntityToQuery(QSqlQuery& query, const WorkOrder& entity) const {}
QString WorkOrderRepository::getInsertQuery() const { return ""; }
QString WorkOrderRepository::getUpdateQuery() const { return ""; }
