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


void WorkOrderController::approvedCreate(const int vendorId, const QString &workOrderName) const
{
    WorkOrder workOrder;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();

    workOrder.setId(id_in_milliseconds);
    workOrder.setGlobalId("123");
    workOrder.setApprovalStatus(true);
    workOrder.setVendorId(vendorId);
    workOrder.setWorkOrderName(workOrderName);
    m_workOrderRepository->saveQML(&workOrder);

}

std::vector<WorkOrder*> WorkOrderController::getWorkOrderList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;
    if (isApproved)
    {
        VendorController vendorController;
        std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);
        std::vector<WorkOrder*> workOrders;

        auto approvedList = m_workOrderRepository->findAllQML();

        foreach (const WorkOrder *order, approvedList)
        {
            int vendorId = order->getVendorId();
            QString description = order->getWorkOrderName();

            QString vendorName = "N/A";
            foreach (const Vendor *vendor, vecVendor)
            {
                if (vendor->getId() == vendorId)
                {
                    vendorName = vendor->getVendorName();
                    break;
                }
            }

            WorkOrder *workOrder = new WorkOrder();

            workOrder->setId(order->getId());
            workOrder->setWorkOrderName(description);
            workOrder->setVendorId(vendorId);
            workOrder->setVendorName(vendorName);

            workOrders.push_back(workOrder);
        }

        return workOrders;
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("WorkOrder");
        std::vector<WorkOrder*> workOrders;
        VendorController vendorController;

        std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);

        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            int draftId = draftEntitys[i]->getId();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto workOrder = new WorkOrder();
                QJsonObject jsonObj = jsonDoc.object();
                workOrder->setId(draftId);
                workOrder->setGlobalId("123");
                workOrder->setApprovalStatus(true);
                workOrder->setVendorId(jsonObj["vendorId"].toInt());

                int vendorId = workOrder->getVendorId();

                QString vendorName = "N/A";
                foreach (const Vendor *vendor, vecVendor)
                {
                  //  qDebug()<<"vid:" <<vendor->getId();
                    if (vendor->getId() == vendorId)
                    {
                        vendorName = vendor->getVendorName();
                        break;
                    }
                }

                workOrder->setWorkOrderName(jsonObj["description"].toString());
                workOrder->setVendorName(vendorName);
                workOrder->setVendorId(jsonObj["vendorId"].toInt());

                QJsonDocument tempDoc(jsonObj["workOrderLineData"].toArray());
                QString rawJsonString = tempDoc.toJson(QJsonDocument::Compact);

                workOrder->setWorkOrderLineData(rawJsonString);

                workOrders.push_back(workOrder);
            }
        }
        return workOrders;
    }
}

