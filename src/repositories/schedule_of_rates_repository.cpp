#include "repositories/schedule_of_rates_repository.h"

ScheduleOfRatesRepository::ScheduleOfRatesRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<ScheduleOfRates> ScheduleOfRatesRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ScheduleOfRates>> ScheduleOfRatesRepository::findAll() { return {}; }
std::vector<ScheduleOfRates*> ScheduleOfRatesRepository::findAllQML() {
    std::vector<ScheduleOfRates*> rates;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM ScheduleOfRates")) {
        while (query.next()) {
            rates.push_back(mapFromQueryQML(query, this));
        }
    }

    return rates;
}
bool ScheduleOfRatesRepository::save(const ScheduleOfRates& entity) { return false; }
bool ScheduleOfRatesRepository::saveQML(ScheduleOfRates* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool ScheduleOfRatesRepository::update(const ScheduleOfRates& entity) { return false; }
bool ScheduleOfRatesRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ScheduleOfRates>> ScheduleOfRatesRepository::findByApprovalStatus(bool status) { return {}; }
QString ScheduleOfRatesRepository::getTableName() const { return "ScheduleOfRates"; }
std::unique_ptr<ScheduleOfRates> ScheduleOfRatesRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

ScheduleOfRates* ScheduleOfRatesRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto scheduleOfRates = new ScheduleOfRates(parent);
    scheduleOfRates->setId(query.value("id").toInt());
    scheduleOfRates->setGlobalId(query.value("global_id").toString());
    scheduleOfRates->setApprovalStatus(query.value("approval_status").toString());
    scheduleOfRates->setScheduleOfRatesName(query.value("schedule_of_rates_name").toString());

    return scheduleOfRates;
}
void ScheduleOfRatesRepository::bindEntityToQuery(QSqlQuery& query, const ScheduleOfRates& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getScheduleOfRatesName());
}
QString ScheduleOfRatesRepository::getInsertQuery() const {
    return "INSERT INTO ScheduleOfRates (global_id, approval_status, schedule_of_rates_name) "
           "VALUES (?, ?, ?)";
}
QString ScheduleOfRatesRepository::getUpdateQuery() const {
    return "UPDATE ScheduleOfRates SET global_id = ?, approval_status = ?, schedule_of_rates_name = ? WHERE id = ?";
}
