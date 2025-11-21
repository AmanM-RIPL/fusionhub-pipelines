#include "repositories/bill_of_quantity_line_repository.h"

BillOfQuantityLineRepository::BillOfQuantityLineRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<BillOfQuantityLine> BillOfQuantityLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BillOfQuantityLine>> BillOfQuantityLineRepository::findAll() { return {}; }
std::vector<BillOfQuantityLine*> BillOfQuantityLineRepository::findAllQML() {
    std::vector<BillOfQuantityLine*> boq;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BillOfQuantity")) {
        while (query.next()) {
            boq.push_back(mapFromQueryQML(query, this));
        }
    }

    return boq;
}
bool BillOfQuantityLineRepository::save(const BillOfQuantityLine& entity) { return false; }
bool BillOfQuantityLineRepository::saveQML(BillOfQuantityLine* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool BillOfQuantityLineRepository::update(const BillOfQuantityLine& entity) { return false; }
bool BillOfQuantityLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BillOfQuantityLine>> BillOfQuantityLineRepository::findByApprovalStatus(bool status) { return {}; }
QString BillOfQuantityLineRepository::getTableName() const { return "BillOfQuantityLine"; }
std::unique_ptr<BillOfQuantityLine> BillOfQuantityLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

BillOfQuantityLine* BillOfQuantityLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto billOfQuantityLine = new BillOfQuantityLine(parent);
    billOfQuantityLine->setId(query.value("id").toInt());
    billOfQuantityLine->setGlobalId(query.value("global_id").toString());
    billOfQuantityLine->setApprovalStatus(query.value("approval_status").toBool());
    billOfQuantityLine->setBillOfQuantityLineName(query.value("description").toString());
    billOfQuantityLine->setId(query.value("bill_of_quantity_id").toInt());

    return billOfQuantityLine;
}
void BillOfQuantityLineRepository::bindEntityToQuery(QSqlQuery& query, const BillOfQuantityLine& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getBillOfQuantityLineName());
    query.addBindValue(entity.getTaskId());
    query.addBindValue(entity.getBillOfQuantityId());
}
QString BillOfQuantityLineRepository::getInsertQuery() const {
    return "INSERT INTO BillOfQuantityLine (global_id, approval_status, description , "
    "task_id, bill_of_quantity_id) "
           "VALUES (?, ?, ?, ?, ?)";
}
QString BillOfQuantityLineRepository::getUpdateQuery() const {
    return "UPDATE BillOfQuantityLine SET global_id = ?, approval_status = ?, description = ?, "
           "bill_of_quantity = ?, task_id = ? WHERE id = ?";
}
