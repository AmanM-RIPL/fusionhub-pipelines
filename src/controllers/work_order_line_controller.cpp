#include "work_order_line_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

WorkOrderLineController::WorkOrderLineController(QObject *parent)
    : QObject{parent},
    m_workOrderLineRepository(RepositoryLocator::instance().workOrderLineRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}
void WorkOrderLineController::create(const int &vendorId, const QString &description, const QVariantList &workOrderLineData) const
{
    QJsonObject jsonObject;
    jsonObject["vendorId"] = vendorId;
    jsonObject["description"] = description;
    QJsonArray lineArray;
    for (const QVariant &item : workOrderLineData) {
        lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    }

    jsonObject["workOrderLineData"] = lineArray;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "WorkOrder:EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["description"] = "Created By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("WorkOrder");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


std::vector<WorkOrderLine*> WorkOrderLineController::getWorkOrderLineList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_workOrderLineRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("WorkOrder");
        std::vector<WorkOrderLine*> workOrderLines;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto workOrderLine = new WorkOrderLine();
                QJsonObject jsonObj = jsonDoc.object();
                workOrderLine->setId(i + 1);
                workOrderLine->setGlobalId("123");
                workOrderLine->setApprovalStatus(true);
                workOrderLine->setTaskId(jsonObj["taskId"].toInt());
                workOrderLine->setDescription(jsonObj["description"].toString());
                workOrderLine->setAmount(jsonObj["amount"].toDouble());
                workOrderLine->setRetentionAmount(jsonObj["retention_amount"].toDouble());
                workOrderLine->setTaxAmount(jsonObj["tax_amount"].toDouble());
                workOrderLine->setTaxWithHolding(jsonObj["tax_with_holding"].toDouble());


                workOrderLines.push_back(workOrderLine);
            }
        }
        return workOrderLines;
    }
}



