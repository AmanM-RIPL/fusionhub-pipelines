#include "repositories/purchase_order_repository.h"
PurchaseOrderRepository::PurchaseOrderRepository() {}
std::unique_ptr<PurchaseOrder> PurchaseOrderRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<PurchaseOrder>> PurchaseOrderRepository::findAll() { return {}; }
bool PurchaseOrderRepository::save(const PurchaseOrder& entity) { return false; }
bool PurchaseOrderRepository::update(const PurchaseOrder& entity) { return false; }
bool PurchaseOrderRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<PurchaseOrder>> PurchaseOrderRepository::findByApprovalStatus(bool status) { return {}; }
QString PurchaseOrderRepository::getTableName() const { return "PurchaseOrder"; }
std::unique_ptr<PurchaseOrder> PurchaseOrderRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void PurchaseOrderRepository::bindEntityToQuery(QSqlQuery& query, const PurchaseOrder& entity) const {}
QString PurchaseOrderRepository::getInsertQuery() const { return ""; }
QString PurchaseOrderRepository::getUpdateQuery() const { return ""; }
