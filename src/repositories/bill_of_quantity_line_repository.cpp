#include "repositories/bill_of_quantity_line_repository.h"
BillOfQuantityLineRepository::BillOfQuantityLineRepository() {}
std::unique_ptr<BillOfQuantityLine> BillOfQuantityLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BillOfQuantityLine>> BillOfQuantityLineRepository::findAll() { return {}; }
bool BillOfQuantityLineRepository::save(const BillOfQuantityLine& entity) { return false; }
bool BillOfQuantityLineRepository::update(const BillOfQuantityLine& entity) { return false; }
bool BillOfQuantityLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BillOfQuantityLine>> BillOfQuantityLineRepository::findByApprovalStatus(bool status) { return {}; }
QString BillOfQuantityLineRepository::getTableName() const { return "BillOfQuantityLine"; }
std::unique_ptr<BillOfQuantityLine> BillOfQuantityLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void BillOfQuantityLineRepository::bindEntityToQuery(QSqlQuery& query, const BillOfQuantityLine& entity) const {}
QString BillOfQuantityLineRepository::getInsertQuery() const { return ""; }
QString BillOfQuantityLineRepository::getUpdateQuery() const { return ""; }
