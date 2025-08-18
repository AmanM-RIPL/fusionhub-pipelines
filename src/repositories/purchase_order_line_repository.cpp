#include "repositories/purchase_order_line_repository.h"
PurchaseOrderLineRepository::PurchaseOrderLineRepository() {}
std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findAll() { return {}; }
bool PurchaseOrderLineRepository::save(const PurchaseOrderLine& entity) { return false; }
bool PurchaseOrderLineRepository::update(const PurchaseOrderLine& entity) { return false; }
bool PurchaseOrderLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<PurchaseOrderLine>> PurchaseOrderLineRepository::findByApprovalStatus(bool status) { return {}; }
QString PurchaseOrderLineRepository::getTableName() const { return "PurchaseOrderLine"; }
std::unique_ptr<PurchaseOrderLine> PurchaseOrderLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void PurchaseOrderLineRepository::bindEntityToQuery(QSqlQuery& query, const PurchaseOrderLine& entity) const {}
QString PurchaseOrderLineRepository::getInsertQuery() const { return ""; }
QString PurchaseOrderLineRepository::getUpdateQuery() const { return ""; }
