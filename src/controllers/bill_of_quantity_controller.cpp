#include "bill_of_quantity_controller.h"
#include "schedule_of_rates_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

BillOfQuantityController::BillOfQuantityController(QObject *parent)
    : QObject{parent},
    m_billOfQuantityRepository(RepositoryLocator::instance().billOfQuantityRepository()),
    m_scheduleOfRatesRepository(RepositoryLocator::instance().scheduleOfRatesRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void BillOfQuantityController::create(const QString &description, const int &scheduleId) const
{
    // BillOfQuantity billOfQuantity;

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // billOfQuantity.setId(id_in_milliseconds);
    // billOfQuantity.setGlobalId("123");
    // billOfQuantity.setApprovalStatus(true);
    // billOfQuantity.setDescription(description);
    // billOfQuantity.setScheduleId(scheduleId);

    // m_billOfQuantityRepository->saveQML(&billOfQuantity);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["description"] = description;
    jsonObject["scheduleId"] = scheduleId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "BillOfQuantity::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("BillOfQuantity");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<BillOfQuantity*> BillOfQuantityController::getBillOfQuantityList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_billOfQuantityRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("BillOfQuantity");
        std::vector<BillOfQuantity*> billOfQuantitys;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto billOfQuantity = new BillOfQuantity();
                QJsonObject jsonObj = jsonDoc.object();
                billOfQuantity->setId(i + 1);
                billOfQuantity->setGlobalId("123");
                billOfQuantity->setApprovalStatus(true);

                billOfQuantity->setDescription(jsonObj["description"].toString());
                billOfQuantity->setScheduleId(jsonObj["scheduleId"].toInt());

                billOfQuantitys.push_back(billOfQuantity);
            }
        }
        return billOfQuantitys;
    }
}

std::vector<ScheduleOfRates*> BillOfQuantityController::getScheduleOfRatesList() const
{
    return m_scheduleOfRatesRepository->findAllQML();
}

