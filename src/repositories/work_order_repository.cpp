#include "repositories/work_order_repository.h"

WorkOrderRepository::WorkOrderRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<WorkOrder> WorkOrderRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkOrder>> WorkOrderRepository::findAll() { return {}; }
std::vector<WorkOrder*> WorkOrderRepository::findAllQML() {
    std::vector<WorkOrder*> vendor;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM WorkOrder")) {
        while (query.next()) {
            vendor.push_back(mapFromQueryQML(query, this));
        }
    }

    return vendor;
}
bool WorkOrderRepository::save(const WorkOrder& entity) { return false; }
bool WorkOrderRepository::saveQML(WorkOrder* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool WorkOrderRepository::update(const WorkOrder& entity) { return false; }
bool WorkOrderRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkOrder>> WorkOrderRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkOrderRepository::getTableName() const { return "WorkOrder"; }
std::unique_ptr<WorkOrder> WorkOrderRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

WorkOrder* WorkOrderRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto workOrder = new WorkOrder(parent);
    workOrder->setId(query.value("id").toInt());
    workOrder->setGlobalId(query.value("global_id").toString());
    workOrder->setApprovalStatus(query.value("approval_status").toBool());
    workOrder->setWorkOrderName(query.value("description").toString());

    return workOrder;
}
void WorkOrderRepository::bindEntityToQuery(QSqlQuery& query, const WorkOrder& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getWorkOrderName());
}
QString WorkOrderRepository::getInsertQuery() const {
    return "INSERT INTO WorkOrder (global_id, approval_status, description) "
           "VALUES (?, ?, ?)";
}
QString WorkOrderRepository::getUpdateQuery() const {
    return "UPDATE WorkOrder SET global_id = ?, approval_status = ?, description = ? WHERE id = ?";
}

