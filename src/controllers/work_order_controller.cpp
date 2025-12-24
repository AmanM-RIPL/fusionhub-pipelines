#include "work_order_controller.h"
#include "vendor_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

WorkOrderController::WorkOrderController(QObject *parent)
    : QObject{parent},
    m_workOrderRepository(RepositoryLocator::instance().workOrderRepository()),
    m_vendorRepository(RepositoryLocator::instance().vendorRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void WorkOrderController::create(const QString &workOrderName, const int vendorId) const
{
    // WorkOrder workOrde

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // workOrder.setId(id_in_milliseconds);
    // workOrder.setGlobalId("123");
    // workOrder.setApprovalStatus(true);
    // workOrder.setDescription(workOrderName);
    // workOrder.setVendorId(vendorId);

    // m_workOrderRepository->saveQML(&workOrder);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["workOrderName"] = workOrderName;
    jsonObject["vendorId"] = vendorId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "WorkOrder::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["workOrderName"] = "Cretaed By User";
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

std::vector<WorkOrder*> WorkOrderController::getWorkOrderList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_workOrderRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("WorkOrder");
        std::vector<WorkOrder*> workOrders;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto workOrder = new WorkOrder();
                QJsonObject jsonObj = jsonDoc.object();
                workOrder->setId(i + 1);
                workOrder->setGlobalId("123");
                workOrder->setApprovalStatus(true);

                //workOrder->setWorkOrderName(jsonObj["workOrderName"].toString());
                workOrder->setWorkOrderName(jsonObj["description"].toString());
                workOrder->setVendorId(jsonObj["vendorId"].toInt());

                QJsonDocument tempDoc(jsonObj["workOrderLineData"].toArray());
                QString rawJsonString = tempDoc.toJson(QJsonDocument::Compact);

                workOrder->setWorkOrderLineData(rawJsonString);
               // qDebug()<< "workOrderController:getWorkOrderList:"<< rawJsonString;

                workOrders.push_back(workOrder);
            }
        }
        return workOrders;
    }
}

std::vector<Vendor*> WorkOrderController::getVendorList() const
{
    return m_vendorRepository->findAllQML();
}

