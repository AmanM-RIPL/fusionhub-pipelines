#include "repositories/work_order_line_repository.h"

WorkOrderLineRepository::WorkOrderLineRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<WorkOrderLine> WorkOrderLineRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<WorkOrderLine>> WorkOrderLineRepository::findAll() { return {}; }

std::vector<WorkOrderLine*> WorkOrderLineRepository::findAllQML() {
    std::vector<WorkOrderLine*> list;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM WorkOrderLine")) {
        while (query.next()) {
            list.push_back(mapFromQueryQML(query, this));
        }
    }

    return list;
}

bool WorkOrderLineRepository::save(const WorkOrderLine& entity) { return false; }

bool WorkOrderLineRepository::saveQML(WorkOrderLine* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool WorkOrderLineRepository::update(const WorkOrderLine& entity) { return false; }

bool WorkOrderLineRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<WorkOrderLine>> WorkOrderLineRepository::findByApprovalStatus(bool status) { return {}; }

QString WorkOrderLineRepository::getTableName() const { return "WorkOrderLine"; }

std::unique_ptr<WorkOrderLine> WorkOrderLineRepository::mapFromQuery(const QSqlQuery& query) const {
    return nullptr;
}

WorkOrderLine* WorkOrderLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto po = new WorkOrderLine(parent);

    po->setId(query.value("id").toInt());
    po->setGlobalId(query.value("global_id").toString());
    po->setApprovalStatus(query.value("approval_status").toBool());
    po->setWorkOrderId(query.value("work_order_id").toInt());
    po->setTaskId(query.value("task_id").toInt());
    po->setDescription(query.value("description").toString());


    return po;
}

void WorkOrderLineRepository::bindEntityToQuery(QSqlQuery& query, const WorkOrderLine& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getWorkOrderId());
    query.addBindValue(entity.getTaskId());
    query.addBindValue(entity.getDescription());

}

QString WorkOrderLineRepository::getInsertQuery() const {
    return "INSERT INTO WorkOrderLine (global_id, approval_status, work_order_id , description , dollar_value ,"
                    "tax_amount, tax_withholding_amount, task_id, retention_amount) "
           "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
}

QString WorkOrderLineRepository::getUpdateQuery() const {
    return "UPDATE WorkOrderLine SET global_id = ?, approval_status = ?, work_order_id=?, description = ?, dollar_value = ?, "
           "tax_amount = ?,tax_withholding_amount = ?, task_id = ?, retention_amount = ? WHERE id = ?";
}


