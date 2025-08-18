#include "repositories/schedule_of_rates_repository.h"
ScheduleOfRatesRepository::ScheduleOfRatesRepository() {}
std::unique_ptr<ScheduleOfRates> ScheduleOfRatesRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ScheduleOfRates>> ScheduleOfRatesRepository::findAll() { return {}; }
bool ScheduleOfRatesRepository::save(const ScheduleOfRates& entity) { return false; }
bool ScheduleOfRatesRepository::update(const ScheduleOfRates& entity) { return false; }
bool ScheduleOfRatesRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ScheduleOfRates>> ScheduleOfRatesRepository::findByApprovalStatus(bool status) { return {}; }
QString ScheduleOfRatesRepository::getTableName() const { return "ScheduleOfRates"; }
std::unique_ptr<ScheduleOfRates> ScheduleOfRatesRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void ScheduleOfRatesRepository::bindEntityToQuery(QSqlQuery& query, const ScheduleOfRates& entity) const {}
QString ScheduleOfRatesRepository::getInsertQuery() const { return ""; }
QString ScheduleOfRatesRepository::getUpdateQuery() const { return ""; }
