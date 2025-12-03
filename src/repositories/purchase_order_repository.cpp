#include "repositories/purchase_order_repository.h"

PurchaseOrderRepository::PurchaseOrderRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<PurchaseOrder> PurchaseOrderRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<PurchaseOrder>> PurchaseOrderRepository::findAll() { return {}; }

std::vector<PurchaseOrder*> PurchaseOrderRepository::findAllQML() {
    std::vector<PurchaseOrder*> list;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM PurchaseOrder")) {
        while (query.next()) {
            list.push_back(mapFromQueryQML(query, this));
        }
    }

    return list;
}

bool PurchaseOrderRepository::save(const PurchaseOrder& entity) { return false; }

bool PurchaseOrderRepository::saveQML(PurchaseOrder* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool PurchaseOrderRepository::update(const PurchaseOrder& entity) { return false; }

bool PurchaseOrderRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<PurchaseOrder>> PurchaseOrderRepository::findByApprovalStatus(bool status) { return {}; }

QString PurchaseOrderRepository::getTableName() const { return "PurchaseOrder"; }

std::unique_ptr<PurchaseOrder> PurchaseOrderRepository::mapFromQuery(const QSqlQuery& query) const {
    return nullptr;
}

PurchaseOrder* PurchaseOrderRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto po = new PurchaseOrder(parent);

    po->setId(query.value("id").toInt());
    po->setGlobalId(query.value("global_id").toString());
    po->setApprovalStatus(query.value("approval_status").toBool());
    po->setVendorId(query.value("vendor_id").toInt());
    // po->setMaterialId(query.value("material_id").toInt());
    // po->setUnitOfMeasurementId(query.value("unit_of_measurement_id").toInt());


    return po;
}

void PurchaseOrderRepository::bindEntityToQuery(QSqlQuery& query, const PurchaseOrder& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getVendorId());
    query.addBindValue(entity.getMaterialId());
    query.addBindValue(entity.getUnitOfMeasurementId());

}

QString PurchaseOrderRepository::getInsertQuery() const {
    return "INSERT INTO PurchaseOrder (global_id, approval_status, vendor_id) "
           "VALUES (?, ?, ?)";
}

QString PurchaseOrderRepository::getUpdateQuery() const {
    return "UPDATE PurchaseOrder SET global_id = ?, approval_status = ?, vendor_id = ?, "
        " WHERE id = ?";
}
