#include "repositories/bill_of_quantity_repository.h"
BillOfQuantityRepository::BillOfQuantityRepository() {}
std::unique_ptr<BillOfQuantity> BillOfQuantityRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BillOfQuantity>> BillOfQuantityRepository::findAll() { return {}; }
bool BillOfQuantityRepository::save(const BillOfQuantity& entity) { return false; }
bool BillOfQuantityRepository::update(const BillOfQuantity& entity) { return false; }
bool BillOfQuantityRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BillOfQuantity>> BillOfQuantityRepository::findByApprovalStatus(bool status) { return {}; }
QString BillOfQuantityRepository::getTableName() const { return "BillOfQuantity"; }
std::unique_ptr<BillOfQuantity> BillOfQuantityRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void BillOfQuantityRepository::bindEntityToQuery(QSqlQuery& query, const BillOfQuantity& entity) const {}
QString BillOfQuantityRepository::getInsertQuery() const { return ""; }
QString BillOfQuantityRepository::getUpdateQuery() const { return ""; }
