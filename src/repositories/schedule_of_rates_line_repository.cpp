#include "repositories/schedule_of_rates_line_repository.h"

ScheduleOfRatesLineRepository::ScheduleOfRatesLineRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<ScheduleOfRatesLine> ScheduleOfRatesLineRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<ScheduleOfRatesLine>> ScheduleOfRatesLineRepository::findAll() { return {}; }

std::vector<ScheduleOfRatesLine*> ScheduleOfRatesLineRepository::findAllQML() {
    std::vector<ScheduleOfRatesLine*> list;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM ScheduleOfRatesLine")) {
        while (query.next()) {
            list.push_back(mapFromQueryQML(query, this));
        }
    }

    return list;
}

bool ScheduleOfRatesLineRepository::save(const ScheduleOfRatesLine& entity) { return false; }

bool ScheduleOfRatesLineRepository::saveQML(ScheduleOfRatesLine* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool ScheduleOfRatesLineRepository::update(const ScheduleOfRatesLine& entity) { return false; }

bool ScheduleOfRatesLineRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<ScheduleOfRatesLine>> ScheduleOfRatesLineRepository::findByApprovalStatus(bool status) { return {}; }

QString ScheduleOfRatesLineRepository::getTableName() const { return "ScheduleOfRatesLine"; }

std::unique_ptr<ScheduleOfRatesLine> ScheduleOfRatesLineRepository::mapFromQuery(const QSqlQuery& query) const {
    return nullptr;
}

ScheduleOfRatesLine* ScheduleOfRatesLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto scheduleOfRatesLine = new ScheduleOfRatesLine(parent);

    scheduleOfRatesLine->setId(query.value("id").toInt());
    scheduleOfRatesLine->setGlobalId(query.value("global_id").toString());
    scheduleOfRatesLine->setApprovalStatus(query.value("approval_status").toBool());
    scheduleOfRatesLine->setScheduleSetupId(query.value("schedule_setup_id").toInt());
    scheduleOfRatesLine->setScheduleOfRatesId(query.value("schedule_of_rates_id").toInt());
    scheduleOfRatesLine->setCostParam(query.value("cost").toString());
    scheduleOfRatesLine->setCostParam(query.value("resource").toString());

    return scheduleOfRatesLine;
}

void ScheduleOfRatesLineRepository::bindEntityToQuery(QSqlQuery& query, const ScheduleOfRatesLine& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getScheduleSetupId());
    query.addBindValue(entity.getScheduleOfRatesId());
    query.addBindValue(entity.getCostParam());
    query.addBindValue(entity.getResourceParam());

}

QString ScheduleOfRatesLineRepository::getInsertQuery() const {
    return "INSERT INTO ScheduleOfRatesLine (global_id, approval_status, schedule_setup_id, schedule_of_rates_id, cost, resource) "
           "VALUES (?, ?, ?, ?, ?)";
}

QString ScheduleOfRatesLineRepository::getUpdateQuery() const {
    return "UPDATE ScheduleOfRatesLine SET global_id = ?, approval_status = ?, schedule_setup_id = ?, schedule_of_rates_id = ?, cost = ?, resource = ?, "
           " WHERE id = ?";
}
