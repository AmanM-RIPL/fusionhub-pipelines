#include "schedule_of_rates_line_controller.h"
#include "schedule_of_rates_controller.h"
//#include "material_controller.h"
//#include "unit_of_measurement_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

ScheduleOfRatesLineController::ScheduleOfRatesLineController(QObject *parent)
    : QObject{parent},
    m_scheduleOfRatesLineRepository(RepositoryLocator::instance().scheduleOfRatesLineRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void ScheduleOfRatesLineController::create(const int &scheduleSetupId, const int &scheduleOfRatesId, const QVariantList &scheduleOfRatesLine) const
{
    // QJsonObject jsonObject;
    // jsonObject["schedule_id"] = scheduleSetupId;
    // jsonObject["schedule_type_id"] = scheduleOfRatesId;

    // QJsonArray lineArray;
    // for (const QVariant &item : scheduleOfRatesLine) {
    //     lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    // }

    // jsonObject["listData"] = lineArray;

    // QJsonDocument jsonDoc(jsonObject);
    // QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);


    // QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    // QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    // QJsonObject jsonObjectChangeHistory;
    // jsonObjectChangeHistory["user"] = gUser->getId();
    // jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    // jsonObjectChangeHistory["changeType"] = "create";
    // jsonObjectChangeHistory["description"] = "Created By User";
    // jsonObjectChangeHistory["approvalHistory"] = "null";

    // QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    // QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    // QDate createdOn = QDate::currentDate();

    // DraftEntity draftEntity;
    // draftEntity.setId(0);
    // draftEntity.setTenant(gTenantId);
    // draftEntity.setCreatedOn(createdOn);
    // draftEntity.setProject(gProjectId);
    // draftEntity.setEntity("ScheduleOfRatesLine");
    // draftEntity.setCreatedByUser(gUser->getId());
    // draftEntity.setNextApprovingUser(0);
    // draftEntity.setEntitySchema(entitySchema);
    // draftEntity.setAssociatedApprovedEntity(0);
    // draftEntity.setChangeHistory(changeHistory);

    // m_draftEntityRepository->saveQML(&draftEntity);

}

void ScheduleOfRatesLineController::approvedCreate(int scheduleSetupId,int scheduleOfRatesId,const QString &cost,const QString &resource) const
{
    ScheduleOfRatesLine line;

    line.setId(0);
    line.setGlobalId("123");
    line.setApprovalStatus(true);

    line.setScheduleSetupId(scheduleSetupId);
    line.setScheduleOfRatesId(scheduleOfRatesId);
    line.setCostParam(cost);
    line.setResourceParam(resource);

    m_scheduleOfRatesLineRepository->saveQML(&line);
}


void ScheduleOfRatesLineController::update(int id, const int &scheduleSetupId, const int &scheduleOfRatesId, const QVariantList &scheduleOfRatesLine) const
{
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["schedule_setup_id"] = scheduleSetupId;
    jsonObject["schedule_of_rates_id"] = scheduleOfRatesId;

    QJsonArray lineArray;
    for (const QVariant &item : scheduleOfRatesLine) {
        lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    }

    jsonObject["listData"] = lineArray;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "PurchaseOrder:EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "update";
    jsonObjectChangeHistory["description"] = "Updated By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate updatedOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(updatedOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("ScheduleOfRatesLine");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);

}

std::vector<ScheduleOfRatesLine*> ScheduleOfRatesLineController::getScheduleOfRatesLineList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    std::vector<ScheduleOfRatesLine*> scheduleOfRatesLines;

    if (isApproved)
    {
        return m_scheduleOfRatesLineRepository->findAllQML();
    }

    return scheduleOfRatesLines;
}

