#include "repositories/schedule_of_rates_line_repository.h"
ScheduleOfRatesLineRepository::ScheduleOfRatesLineRepository() {}
std::unique_ptr<ScheduleOfRatesLine> ScheduleOfRatesLineRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ScheduleOfRatesLine>> ScheduleOfRatesLineRepository::findAll() { return {}; }
bool ScheduleOfRatesLineRepository::save(const ScheduleOfRatesLine& entity) { return false; }
bool ScheduleOfRatesLineRepository::update(const ScheduleOfRatesLine& entity) { return false; }
bool ScheduleOfRatesLineRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ScheduleOfRatesLine>> ScheduleOfRatesLineRepository::findByApprovalStatus(bool status) { return {}; }
QString ScheduleOfRatesLineRepository::getTableName() const { return "ScheduleOfRatesLine"; }
std::unique_ptr<ScheduleOfRatesLine> ScheduleOfRatesLineRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void ScheduleOfRatesLineRepository::bindEntityToQuery(QSqlQuery& query, const ScheduleOfRatesLine& entity) const {}
QString ScheduleOfRatesLineRepository::getInsertQuery() const { return ""; }
QString ScheduleOfRatesLineRepository::getUpdateQuery() const { return ""; }
