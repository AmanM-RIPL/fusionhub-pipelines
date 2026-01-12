#include "schedule_setup_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

ScheduleSetupController::ScheduleSetupController(QObject *parent)
    : QObject{parent},
    m_scheduleSetupRepository(RepositoryLocator::instance().scheduleSetupRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

//void ScheduleSetupController::create(const QString &scheduleName, const QString &description, const QString &costParameter,
//                              const QString &resourceParameter) const

void ScheduleSetupController::create(const QString &scheduleName, const QString &description, const QVariant &costParameter,
                                     const QVariant &resourceParameter) const
{

    QString costParam =  CreateJson(costParameter);
    qDebug() <<"Created costParam:" << costParam;
    QString resourceParam =  CreateJson(resourceParameter);
    qDebug() <<"Created resourceParam:" << resourceParam;


    ScheduleSetup setup;
    setup.setId(0);
    setup.setGlobalId("123");
    setup.setApprovalStatus(true);
    setup.setScheduleName(scheduleName);
    setup.setDescription(description);
    setup.setCostParameter(costParam);
    setup.setResourceParameter(resourceParam);


    m_scheduleSetupRepository->saveQML(&setup);


    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = true;
    jsonObject["scheduleName"] = scheduleName;
    jsonObject["description"] = description;
    jsonObject["costParam"] = costParam;
    jsonObject["resourceParam"] = resourceParam;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "ScheduleSetup:EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    //jsonObjectChangeHistory["user"] = gUser->getUserId();
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
    draftEntity.setEntity("ScheduleSetup");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<ScheduleSetup*> ScheduleSetupController::getSetupList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    std::vector<ScheduleSetup*> scheduleSetups ={};

    if(isApproved)
    {
        scheduleSetups =  m_scheduleSetupRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("ScheduleSetup");
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto scheduleSetup = new ScheduleSetup();
                QJsonObject jsonObj = jsonDoc.object();
                scheduleSetup->setId(i + 1);
                scheduleSetup->setGlobalId("123");
                scheduleSetup->setApprovalStatus(true);
                scheduleSetup->setScheduleName(jsonObj["scheduleName"].toString());
                scheduleSetup->setDescription(jsonObj["description"].toString());
                scheduleSetup->setCostParameter(jsonObj["costParam"].toString());
                scheduleSetup->setResourceParameter(jsonObj["resourceParam"].toString());

                scheduleSetups.push_back(scheduleSetup);
            }
        }
    }


    for(int i = 0; i < scheduleSetups.size(); i++)
    {
        QString  jsonStringCostParam = scheduleSetups[i]->getCostParameter();
        QString  jsonStringResourceParam = scheduleSetups[i]->getResourceParameter();

        QJsonDocument jsonDocCostParam = QJsonDocument::fromJson(jsonStringCostParam.toUtf8());
        if (!jsonDocCostParam.isNull())
        {
            if (jsonDocCostParam.isObject()) {
                QJsonObject jsonObj = jsonDocCostParam.object();
                int totalRow = jsonObj["rows"].toInt();
                QString strRows = QString::number(totalRow);
                scheduleSetups[i]->setCostParameter(strRows);
              //  qDebug() << "jsonStringCostParam:" << strRows;

                QVector<costParamDataDetails*> costDetails;

                QJsonArray dataArray = jsonObj["data"].toArray();

                for (const QJsonValue &value : dataArray) {
                    QJsonObject obj = value.toObject();
                    costParamDataDetails* info = new costParamDataDetails();
                    info->setCostParam(obj["cost_param_name"].toString());
                    info->setMaterialParam(obj["purchase_material"].toString());
                    info->setCostBimParam(obj["type_of_bim_dimension"].toString());
                    costDetails.push_back(info);
                }

                qDebug() << "costDetails.length:" << costDetails.size();
                scheduleSetups[i]->setCostParameterDataDetails(costDetails);
            }
        }

        QJsonDocument jsonDocResourceParam = QJsonDocument::fromJson(jsonStringResourceParam.toUtf8());
        if (!jsonDocResourceParam.isNull())
        {
            if (jsonDocResourceParam.isObject()) {
                QJsonObject jsonObj = jsonDocResourceParam.object();
                int totalRow = jsonObj["rows"].toInt();
                QString strRows = QString::number(totalRow);
                scheduleSetups[i]->setResourceParameter(strRows);
                //qDebug() << "jsonStringResourceParam:" << strRows;

                QVector<resourceParamDataDetails*> resourceDetails;

                QJsonArray dataArray = jsonObj["data"].toArray();

                for (const QJsonValue &value : dataArray) {
                    QJsonObject obj = value.toObject();
                    resourceParamDataDetails* info = new resourceParamDataDetails();
                    info->setResourceParam(obj["resource_param_name"].toString());
                    info->setResourceBimParam(obj["type_of_bim_dimension"].toString());
                    resourceDetails.push_back(info);
                }

                qDebug() << "resourceDetails.length:" << resourceDetails.size();
                scheduleSetups[i]->setResourceParameterDataDetails(resourceDetails);
            }
        }
    }
    return scheduleSetups;
}

QString ScheduleSetupController::CreateJson(const QVariant &param) const
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
        qDebug() <<"Created ScheduleSetUp:" << JsonString;

        return JsonString;
    }
}

