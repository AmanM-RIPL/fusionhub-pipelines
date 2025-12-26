#include "work_order_line_controller.h"
#include "common/repository_locator.h"
#include "task_controller.h"
#include "vendor_controller.h"
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
        TaskController taskController;
        VendorController  vendorController;

        std::vector<Task*> vecTask = taskController.getTaskList(true);
        std::vector<Vendor*> vecVendor =  vendorController.getVendorList(true);

        std::vector<WorkOrderLine*> workOrderLines;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            int draftId = draftEntitys[i]->getId();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QByteArray jsonData = jsonString.toUtf8();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                QJsonObject mainObject = jsonDoc.object();
                QString description = mainObject["description"].toString();
                int vendorId = mainObject["vendorId"].toInt();

                QJsonArray lineDataArray = mainObject["workOrderLineData"].toArray();
                foreach (const QJsonValue & value, lineDataArray) {
                    QJsonObject lineItem = value.toObject();

                    QString amount = lineItem["amount"].toString();
                    QString itemDescription = lineItem["description"].toString();
                    QString retentionAmount = lineItem["retention_amount"].toString();
                    QString taskId = lineItem["task_id"].toString();
                    //QString taskName = lineItem["task_name"].toString();
                    QString taxAmount = lineItem["tax_amount"].toString();
                    QString taxWithHolding = lineItem["tax_with_holding"].toString();

                    auto workOrderLine = new WorkOrderLine();
                    workOrderLine->setId(draftId);
                    workOrderLine->setDescription(description);
                    workOrderLine->setAmount(amount.toDouble());
                   // workOrderLine->setDescription(itemDescription);
                    workOrderLine->setRetentionAmount(retentionAmount.toDouble());

                    foreach (const Task *task, vecTask)
                    {
                        if(task->getId() == taskId.toInt())
                        {
                            workOrderLine->setTaskName(task->getTaskName());
                        }
                    }


                    foreach (const Vendor *vendor, vecVendor)
                    {
                        if(vendor->getId() == vendorId)
                        {
                           QString str = vendor->getVendorName();
                           workOrderLine->setVendorName(str);
                        }
                    }

                    workOrderLine->setTaxAmount(taxAmount.toDouble());
                    workOrderLine->setTaxWithHolding(taxWithHolding.toDouble());

                    workOrderLines.push_back(workOrderLine);
                }
            }
        }
        return workOrderLines;
    }
}






