#include "schedule_of_rates_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

ScheduleOfRatesController::ScheduleOfRatesController(QObject *parent)
    : QObject{parent},
    m_scheduleOfRatesRepository(RepositoryLocator::instance().scheduleOfRatesRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void ScheduleOfRatesController::create(const QString &name, const QVariant &costValueParameter) const
{
    // ScheduleOfRates scheduleOfRates;

    /***********Start of DraftEntity******************/

    QString costValueParam =  CreateJson(costValueParameter);

    QJsonObject jsonObject;
    jsonObject["scheduleOfRatesName"] = name;
    jsonObject["costValueParameter"] = costValueParam;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "ScheduleOfRates::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["description"] = "Cretaed By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("ScheduleOfRates");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<ScheduleOfRates*> ScheduleOfRatesController::getScheduleOfRatesList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){

           qDebug()<<"Check: "<< m_scheduleOfRatesRepository->findAllQML();
        return m_scheduleOfRatesRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("ScheduleOfRates");
        std::vector<ScheduleOfRates*> scheduleOfRatess;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto scheduleOfRates = new ScheduleOfRates();
                QJsonObject jsonObj = jsonDoc.object();
                scheduleOfRates->setId(i + 1);
                scheduleOfRates->setGlobalId("123");
                scheduleOfRates->setApprovalStatus(true);

                scheduleOfRates->setScheduleOfRatesName(jsonObj["scheduleOfRatesName"].toString());
                scheduleOfRatess.push_back(scheduleOfRates);
            }
        }
        return scheduleOfRatess;
    }
}

QString ScheduleOfRatesController::CreateJson(const QVariant &param) const
{
    QJsonObject jsonObject;
    if (param.canConvert<QVariantList>()) {
        QVariantList list = param.toList();
        int len = list.size();
        jsonObject["rows"] = len;

        QJsonArray dataArray;

        for (const QVariant &item : list) {
            qDebug() << "Item:" << item.toString();

            if (item.canConvert<QVariantMap>()) {
                QVariantMap map = item.toMap();
                QJsonObject jsonObjectNew;

                // Iterate through the map to populate the QJsonObject
                for (auto it = map.begin(); it != map.end(); ++it) {
                    jsonObjectNew.insert(it.key(), QJsonValue::fromVariant(it.value()));
                }

                dataArray.append(jsonObjectNew);
            } else {
                qDebug() << "Item is not a QVariantMap!";
            }
        }

        jsonObject["data"] = dataArray;
        QJsonDocument jsonDoc(jsonObject);
        QString JsonString = jsonDoc.toJson(QJsonDocument::Indented);
        qDebug() <<"Created ScheduleOfRates:" << JsonString;

        return JsonString;
    }
}


