#include "schedule_of_rates_controller.h"
#include "schedule_of_rates_line_controller.h"
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

void ScheduleOfRatesController::create(const QString &name, const QVariant &pramMainMap) const
{

    /***********Start of DraftEntity******************/

    QJsonDocument scheduleOfRatesLineDoc =  CreateJson(pramMainMap);

    QJsonObject jsonObject;
    jsonObject["scheduleOfRatesName"] = name;
    //jsonObject["costValueParameter"] = costValueParam;
    jsonObject["scheduleOfRatesLine"] = scheduleOfRatesLineDoc.array();

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
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


void ScheduleOfRatesController::approvedCreate(const QString &name) const
{
    ScheduleOfRates scheduleOfRates;

    scheduleOfRates.setId(0);
    scheduleOfRates.setGlobalId("123");
    // setApprovalStatus(true);

    scheduleOfRates.setScheduleOfRatesName(name);

    m_scheduleOfRatesRepository->saveQML(&scheduleOfRates);


}

std::vector<ScheduleOfRates*> ScheduleOfRatesController::getScheduleOfRatesList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){

          // qDebug()<<"Check: "<< m_scheduleOfRatesRepository->findAllQML();
        return m_scheduleOfRatesRepository->findAllQML();
    }
    else {
        std::vector<DraftEntity*> draftEntitys =
            m_draftEntityRepository->findAllQML("ScheduleOfRates");

        std::vector<ScheduleOfRates*> scheduleOfRatess;

        for (int i = 0; i < draftEntitys.size(); i++) {

            int draftId = draftEntitys[i]->getId();
            QString jsonString = draftEntitys[i]->getEntitySchema();
            QString approvalStatus = draftEntitys[i]->getApprovalStatus();
            int nextApprovingUser = draftEntitys[i]->getNextApprovingUser();
            int createdByUser = draftEntitys[i]->getCreatedByUser();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

            if (!jsonDoc.isNull() && jsonDoc.isObject()) {

                QJsonObject jsonObj = jsonDoc.object();

                auto scheduleOfRates = new ScheduleOfRates();

                scheduleOfRates->setId(draftId);
                scheduleOfRates->setGlobalId("123");
                scheduleOfRates->setApprovalStatus(approvalStatus);
                scheduleOfRates->setCreatedByUser(createdByUser);
                scheduleOfRates->setNextApprovingUser(nextApprovingUser);

                // ===== Main Name =====
                scheduleOfRates->setScheduleOfRatesName(jsonObj["scheduleOfRatesName"].toString());

                // ===== Schedule Of Rates Line Array =====
                QJsonArray lineArray = jsonObj["scheduleOfRatesLine"].toArray();

                QList<QObject*> lines;

                for (int j = 0; j < lineArray.size(); j++) {

                    QJsonObject lineObj = lineArray[j].toObject();

                    int scheduleSetupId = lineObj["scheduleSetupId"].toString().toInt();

                    QJsonArray dataArray = lineObj["data"].toArray();


                    for (int k = 0; k < dataArray.size(); k++) {

                        QJsonObject dataObj = dataArray[k].toObject();

                        auto* line = new ScheduleOfRatesLine();

                        // line->setId(0); // draft child row
                        // line->setScheduleOfRatesId(draftId);
                        line->setScheduleSetupId(scheduleSetupId);

                        line->setCostParam(dataObj["cost"].toString());
                        line->setResourceParam(dataObj["value"].toString());



                        lines.push_back(line);
                    }
                }

                scheduleOfRates->setScheduleOfRatesLines(lines);

                scheduleOfRatess.push_back(scheduleOfRates);
            }
        }

        return scheduleOfRatess;
    }

}

QJsonDocument ScheduleOfRatesController::CreateJson(const QVariant &paramMap) const
{    
    if (paramMap.canConvert<QVariantMap>())
    {
        QVariantMap mainMap = paramMap.toMap();

        QJsonArray dataArrayMain;
        for (auto itr = mainMap.begin(); itr != mainMap.end(); ++itr)
        {
            QVariant param = itr.value();

            QJsonObject jsonObject;
            if (param.canConvert<QVariantList>()) {
                QVariantList list = param.toList();
                int len = list.size();
                jsonObject["scheduleSetupId"] = itr.key();
                if(len <= 0 )
                    continue;

                jsonObject["rows"] = len;

                QJsonArray dataArray;
                for (const QVariant &item : list) {
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
            }

            if(!jsonObject.isEmpty())
            {
                dataArrayMain.append(jsonObject);
            }
        }
        QJsonDocument jsonDoc(dataArrayMain);
        return jsonDoc;
    }
    QJsonDocument emptyDoc;
    return emptyDoc;
}


