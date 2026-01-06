#include "work_billing_controller.h"
#include "work_order_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

WorkBillingController::WorkBillingController(QObject *parent)
    : QObject{parent},
    m_workBillingRepository(RepositoryLocator::instance().workBillingRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void WorkBillingController::create(const QString &workBillingName, const int workOrderId) const
{
    // WorkBilling workOrde

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // workBilling.setId(id_in_milliseconds);
    // workBilling.setGlobalId("123");
    // workBilling.setApprovalStatus(true);
    // workBilling.setDescription(workBillingName);
    // workBilling.setWorkOrderId(workOrderId);

    // m_workBillingRepository->saveQML(&workBilling);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["description"] = workBillingName;
    jsonObject["workOrderId"] = workOrderId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "WorkBilling::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["workBillingName"] = "Cretaed By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("WorkBilling");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<WorkBilling*> WorkBillingController::getWorkBillingList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;
    if (isApproved)
    {
        WorkOrderController workOrderController;
        std::vector<WorkOrder*> vecWorkOrder = workOrderController.getWorkOrderList(true);
        std::vector<WorkBilling*> workBillings;

        auto approvedList = m_workBillingRepository->findAllQML();

        foreach (const WorkBilling *order, approvedList)
        {
            int workOrderId = order->getWorkOrderId();
            QString description = order->getWorkBillingName();

            QString workOrderName = "N/A";
            foreach (const WorkOrder *workOrder, vecWorkOrder)
            {
                if (workOrder->getId() == workOrderId)
                {
                    workOrderName = workOrder->getWorkOrderName();
                    break;
                }
            }

            WorkBilling *workBilling = new WorkBilling();

            workBilling->setId(order->getId());
            workBilling->setWorkBillingName(description);
            workBilling->setWorkOrderId(workOrderId);
            workBilling->setWorkOrderName(workOrderName);

            workBillings.push_back(workBilling);
        }

        return workBillings;
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("WorkBilling");
        std::vector<WorkBilling*> workBillings;
        WorkOrderController workOrderController;

        std::vector<WorkOrder*> vecWorkOrder = workOrderController.getWorkOrderList(true);

        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            int draftId = draftEntitys[i]->getId();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto workBilling = new WorkBilling();
                QJsonObject jsonObj = jsonDoc.object();
                workBilling->setId(draftId);
                workBilling->setGlobalId("123");
                workBilling->setApprovalStatus(true);
                workBilling->setWorkOrderId(jsonObj["workOrderId"].toInt());

                int workOrderId = workBilling->getWorkOrderId();
                QString workBillingName = jsonObj["description"].toString("");
                QString workOrderName = "N/A";
                foreach (const WorkOrder *workOrder, vecWorkOrder)
                {
                      qDebug()<<"woid:" <<workOrder->getId();
                    if (workOrder->getId() == workOrderId)
                    {
                        workOrderName = workOrder->getWorkOrderName();
                        break;
                    }
                }

                workBilling->setWorkBillingName(workBillingName);
                workBilling->setWorkOrderName(workOrderName);
              //  workBilling->setWorkOrderId(jsonObj["workOrderId"].toInt());


                workBillings.push_back(workBilling);
            }
        }
        return workBillings;
    }
}

