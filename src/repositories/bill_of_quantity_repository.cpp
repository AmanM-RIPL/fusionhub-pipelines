#include "repositories/bill_of_quantity_repository.h"
#include "repositories/schedule_of_rates_repository.h"

BillOfQuantityRepository::BillOfQuantityRepository(QObject* parent) : QObject(parent) {}
std::unique_ptr<BillOfQuantity> BillOfQuantityRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BillOfQuantity>> BillOfQuantityRepository::findAll() { return {}; }
std::vector<BillOfQuantity*> BillOfQuantityRepository::findAllQML() {
    std::vector<BillOfQuantity*> schedule;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BillOfQuantity")) {
        while (query.next()) {
            schedule.push_back(mapFromQueryQML(query, this));
        }
    }

    return schedule;
}
bool BillOfQuantityRepository::save(const BillOfQuantity& entity) { return false; }
bool BillOfQuantityRepository::saveQML(BillOfQuantity* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool BillOfQuantityRepository::update(const BillOfQuantity& entity) { return false; }
bool BillOfQuantityRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BillOfQuantity>> BillOfQuantityRepository::findByApprovalStatus(bool status) { return {}; }
QString BillOfQuantityRepository::getTableName() const { return "BillOfQuantity"; }
std::unique_ptr<BillOfQuantity> BillOfQuantityRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

BillOfQuantity* BillOfQuantityRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto billOfQuantity = new BillOfQuantity(parent);
    billOfQuantity->setId(query.value("id").toInt());
    billOfQuantity->setGlobalId(query.value("global_id").toString());
    billOfQuantity->setApprovalStatus(query.value("approval_status").toString());
    billOfQuantity->setDescription(query.value("description").toString());
    billOfQuantity->setScheduleOfRatesId(query.value("schedule_of_rates_id").toInt());

    return billOfQuantity;
}
void BillOfQuantityRepository::bindEntityToQuery(QSqlQuery& query, const BillOfQuantity& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getDescription());
    query.addBindValue(entity.getScheduleOfRatesId());
}
QString BillOfQuantityRepository::getInsertQuery() const {
    return "INSERT INTO BillOfQuantity (global_id, approval_status, description, "
           "schedule_of_rates_id) "
           "VALUES (?, ?, ?, ?)";
}
QString BillOfQuantityRepository::getUpdateQuery() const {
    return "UPDATE BillOfQuantity SET global_id = ?, approval_status = ?, description = ? , schedule_of_rates_id = ? WHERE id = ?";
}
