#include "repositories/work_billing_repository.h"

WorkBillingRepository::WorkBillingRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<WorkBilling> WorkBillingRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<WorkBilling>> WorkBillingRepository::findAll() { return {}; }
std::vector<WorkBilling*> WorkBillingRepository::findAllQML() {
    std::vector<WorkBilling*> workOrder;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM WorkBilling")) {
        while (query.next()) {
            workOrder.push_back(mapFromQueryQML(query, this));
        }
    }

    return workOrder;
}
bool WorkBillingRepository::save(const WorkBilling& entity) { return false; }
bool WorkBillingRepository::saveQML(WorkBilling* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool WorkBillingRepository::update(const WorkBilling& entity) { return false; }
bool WorkBillingRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<WorkBilling>> WorkBillingRepository::findByApprovalStatus(bool status) { return {}; }
QString WorkBillingRepository::getTableName() const { return "WorkBilling"; }
std::unique_ptr<WorkBilling> WorkBillingRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

WorkBilling* WorkBillingRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto workBilling = new WorkBilling(parent);
    workBilling->setId(query.value("id").toInt());
    workBilling->setGlobalId(query.value("global_id").toString());
    workBilling->setApprovalStatus(query.value("approval_status").toBool());
    workBilling->setWorkBillingName(query.value("description").toString());
    workBilling->setWorkOrderId(query.value("work_order_id").toInt());

    return workBilling;
}
void WorkBillingRepository::bindEntityToQuery(QSqlQuery& query, const WorkBilling& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getWorkBillingName());
}
QString WorkBillingRepository::getInsertQuery() const {
    return "INSERT INTO WorkBilling (global_id, approval_status, description) "
           "VALUES (?, ?, ?)";
}
QString WorkBillingRepository::getUpdateQuery() const {
    return "UPDATE WorkBilling SET global_id = ?, approval_status = ?, description = ? WHERE id = ?";
}


