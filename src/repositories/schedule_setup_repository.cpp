#include "repositories/schedule_setup_repository.h"
ScheduleSetupRepository::ScheduleSetupRepository(QObject* parent) : QObject(parent) {}
std::unique_ptr<ScheduleSetup> ScheduleSetupRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<ScheduleSetup>> ScheduleSetupRepository::findAll() { return {}; }
std::vector<ScheduleSetup*> ScheduleSetupRepository::findAllQML() {
    std::vector<ScheduleSetup*> scheduleSetup;
    QSqlQuery query(dbManager->getDatabase());
    if (query.exec("SELECT * FROM ScheduleSetup")) {
        while (query.next()) {
            scheduleSetup.push_back(mapFromQueryQML(query, this));
        }
    }
    return scheduleSetup;
}

bool ScheduleSetupRepository::save(const ScheduleSetup& entity) { return false; }
bool ScheduleSetupRepository::saveQML(ScheduleSetup* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool ScheduleSetupRepository::update(const ScheduleSetup& entity) { return false; }
bool ScheduleSetupRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<ScheduleSetup>> ScheduleSetupRepository::findByApprovalStatus(bool status) { return {}; }
QString ScheduleSetupRepository::getTableName() const { return "ScheduleSetup"; }
std::unique_ptr<ScheduleSetup> ScheduleSetupRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
ScheduleSetup* ScheduleSetupRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto scheduleSetup = new ScheduleSetup(parent);
    scheduleSetup->setId(query.value("id").toInt());
    scheduleSetup->setGlobalId(query.value("global_id").toString());
    scheduleSetup->setApprovalStatus(query.value("approval_status").toBool());
    scheduleSetup->setScheduleSetupName(query.value("schedule_setup_name").toString());
    scheduleSetup->setDescription(query.value("description").toString());
    scheduleSetup->setCostParameter(query.value("cost_parameter").toString());
    scheduleSetup->setResourceParameter(query.value("resource_parameter").toString());

    return scheduleSetup;
}
void ScheduleSetupRepository::bindEntityToQuery(QSqlQuery& query, const ScheduleSetup& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getScheduleSetupName());
    query.addBindValue(entity.getDescription());
    query.addBindValue(entity.getCostParameter());
    query.addBindValue(entity.getResourceParameter());
}
QString ScheduleSetupRepository::getInsertQuery() const {
    return "INSERT INTO ScheduleSetup (global_id, approval_status, schedule_setup_name, description, "
           "cost_parameter, resource_parameter) "
           "VALUES (?, ?, ?, ?, ?, ?)";
}
QString ScheduleSetupRepository::getUpdateQuery() const {
    return "UPDATE ScheduleSetup SET global_id = ?, approval_status = ?, schedule_setup_name = ?, "
           "description = ?, cost_parameter = ?, resource_parameter = ? WHERE id = ?";
}
