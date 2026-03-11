#include "repositories/purchase_order_line_repository.h"

PurchaseOrderLineRepository::PurchaseOrderLineRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findAll() { return {}; }

std::vector<PurchaseOrderLine*> PurchaseOrderLineRepository::findAllQML() {
    std::vector<PurchaseOrderLine*> list;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM PurchaseOrderLine")) {
        while (query.next()) {
            list.push_back(mapFromQueryQML(query, this));
        }
    }

    return list;
}

bool PurchaseOrderLineRepository::save(const PurchaseOrderLine& entity) { return false; }

bool PurchaseOrderLineRepository::saveQML(PurchaseOrderLine* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool PurchaseOrderLineRepository::update(const PurchaseOrderLine& entity) { return false; }

bool PurchaseOrderLineRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findByApprovalStatus(bool status) { return {}; }

QString PurchaseOrderLineRepository::getTableName() const { return "PurchaseOrderLine"; }

std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::mapFromQuery(const QSqlQuery& query) const {
    return nullptr;
}

PurchaseOrderLine* PurchaseOrderLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto po = new PurchaseOrderLine(parent);

    po->setId(query.value("id").toInt());
    po->setGlobalId(query.value("global_id").toString());
    po->setApprovalStatus(query.value("approval_status").toBool());
    po->setPurchaseOrderId(query.value("purchase_order_id").toInt());
    po->setMaterialId(query.value("material_id").toInt());
    po->setUnitOfMeasurementId(query.value("unit_of_measurement_id").toInt());
    po->setQuantity(query.value("quantity").toDouble());
    po->setAmount(query.value("dollar_value").toDouble());


    return po;
}

void PurchaseOrderLineRepository::bindEntityToQuery(QSqlQuery& query, const PurchaseOrderLine& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getPurchaseOrderId());
    query.addBindValue(entity.getMaterialId());
    query.addBindValue(entity.getUnitOfMeasurementId());

}

QString PurchaseOrderLineRepository::getInsertQuery() const {
    return "INSERT INTO PurchaseOrderLine (global_id, approval_status, vendor_id) "
           "VALUES (?, ?, ?)";
}

QString PurchaseOrderLineRepository::getUpdateQuery() const {
    return "UPDATE PurchaseOrderLine SET global_id = ?, approval_status = ?, vendor_id = ?, "
           " WHERE id = ?";
}

