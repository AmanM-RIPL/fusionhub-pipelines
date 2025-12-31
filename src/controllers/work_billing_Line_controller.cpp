#include "work_billing_line_controller.h"
#include "work_order_controller.h"
#include "work_order_line_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include "task_controller.h"
#include "work_order_controller.h"

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

WorkBillingLineController::WorkBillingLineController(QObject *parent)
    : QObject{parent},
    m_workBillingLineRepository(RepositoryLocator::instance().workBillingLineRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}


void WorkBillingLineController::create(const int &workOrderId, const QVariantList &workBillingLine) const
{
    QJsonObject jsonObject;
    jsonObject["workOrderId"] = workOrderId;
    QJsonArray lineArray;
    for (const QVariant &item : workBillingLine) {
        lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    }

    jsonObject["listData"] = lineArray;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "WorkBilling:EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("WorkBillingLine");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<WorkBillingLine*> WorkBillingLineController::getWorkBillingLineList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;
    if (isApproved)
    {
        return m_workBillingLineRepository->findAllQML();
    }

    // ---- Fetch Draft WorkBillingLine Data ----
    std::vector<DraftEntity*> draftEntitys = m_draftEntityRepository->findAllQML("WorkBillingLine");

    // Reference data for mapping
    WorkOrderController workOrderController;
    WorkOrderLineController workOrderLineController;

    std::vector<WorkOrder*> vecWorkOrder = workOrderController.getWorkOrderList(isApproved);
    std::vector<WorkOrderLine*> vecWorkOrderLine = workOrderLineController.getWorkOrderLineList(isApproved);

    std::vector<WorkBillingLine*> workBillingLines;

    for (int i = 0; i < draftEntitys.size(); i++)
    {
        QString jsonString = draftEntitys[i]->getEntitySchema();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject mainObject = jsonDoc.object();

            int workOrderId = mainObject["workOrderId"].toInt();
            QJsonArray lineArray = mainObject["listData"].toArray();

            foreach(const QJsonValue &value, lineArray)
            {
                QJsonObject obj = value.toObject();

                QString amount              = obj["amount"].toString();
                QString retentionAmount     = obj["retention_amount"].toString();
                QString taxAmount           = obj["tax_amount"].toString();
                QString taxWithHolding      = obj["tax_with_holding"].toString();
                int workOrderLineId     = obj["work_order_line_id"].toInt();

                auto workBilling = new WorkBillingLine();

                // Assign values
                workBilling->setAmount(amount.toDouble());
                workBilling->setRetentionAmount(retentionAmount.toDouble());
                workBilling->setTaxAmount(taxAmount.toDouble());
                workBilling->setTaxWithHolding(taxWithHolding.toDouble());

                // Store IDs (important for QML binding)
                workBilling->setWorkOrderId(workOrderId);
                workBilling->setWorkOrderLineId(workOrderLineId);

                // ---- Match Work Order  ----
                foreach(const WorkOrder *wo, vecWorkOrder)
                {
                    if (wo->getId() == workOrderId)
                    {
                        workBilling->setWorkOrderName(wo->getWorkOrderName());
                        break;
                    }
                }

                // ---- Match Work Order Line ----
                foreach(const WorkOrderLine *wol, vecWorkOrderLine)
                {
                    if (wol->getId() == workOrderLineId)
                    {
                        workBilling->setWorkOrderLineName(wol->getDescription());
                        break;
                    }
                }

                workBillingLines.push_back(workBilling);
            }
        }
    }

    return workBillingLines;
}


QString WorkBillingLineController::CreateJson(const QVariant &param) const
{
    QJsonObject jsonObject;
    if (param.canConvert<QVariantList>())
    {
        QVariantList list = param.toList();
        int len = list.size();
        jsonObject["rows"] = len;

        QJsonArray dataArray;

        for (const QVariant &item : list)
        {
            qDebug() << "Item:" << item.toString();

            if (item.canConvert<QVariantMap>())
            {
                QVariantMap map = item.toMap();
                QJsonObject jsonObjectNew;

                // Iterate through the map to populate the QJsonObject
                for (auto it = map.begin(); it != map.end(); ++it)
                {
                    jsonObjectNew.insert(it.key(), QJsonValue::fromVariant(it.value()));
                }

                dataArray.append(jsonObjectNew);
            }
            else
            {
                qDebug() << "Item is not a QVariantMap!";
            }
        }

        jsonObject["data"] = dataArray;
        QJsonDocument jsonDoc(jsonObject);
        QString JsonString = jsonDoc.toJson(QJsonDocument::Indented);
        qDebug() <<"Created :" << JsonString;

        return JsonString;
    }

    return QString();
}

std::vector<Task*> WorkBillingLineController::getBilledTaskList(bool isApproved) const
{
    std::vector<Task*> tasklist;
    WorkOrderController workOrderController;
    WorkOrderLineController workOrderLineController;
    TaskController taskController;

    std::vector<WorkBillingLine*> workBillingLines = getWorkBillingLineList(isApproved);
    std::vector<WorkOrder*> workOrderList = workOrderController.getWorkOrderList(isApproved);
    std::vector<Task*> taskList = taskController.getTaskList(isApproved);


    if(isApproved)
    {
        std::vector<WorkOrderLine*> workOrderLine = workOrderLineController.getWorkOrderLineList(isApproved);

        foreach (const WorkBillingLine *workbill, workBillingLines)
        {
            int workOrderLineId = workbill->getWorkOrderLineId();
            foreach (const WorkOrderLine *workorderline, workOrderLine)
            {
                int workOrderLineIdNew = workorderline->getId();
                if(workOrderLineId == workOrderLineIdNew)
                {
                    int taskId = workorderline->getTaskId();
                    foreach (Task *task, taskList)
                    {
                        int taskIdNew = task->getId();
                        if(taskId == taskIdNew)
                        {
                            tasklist.push_back(task);
                        }
                    }
                }
            }
        }
    }
    else
    {
        foreach (const WorkBillingLine *workbill, workBillingLines)
        {
            int workOrderId = workbill->getWorkOrderId();
            foreach (const WorkOrder *workorder, workOrderList)
            {
                int workOrderIdNew = workorder->getId();
                if(workOrderId == workOrderIdNew)
                {
                    QString workOrderLineData =  workorder->getWorkOrderLineData();
                    QJsonParseError parseError;
                    QJsonDocument doc = QJsonDocument::fromJson(workOrderLineData.toUtf8(), &parseError);
                    if (!doc.isNull() && doc.isArray()) {
                        QJsonArray lineArray = doc.array();
                        for (const QJsonValue &value : lineArray) {
                            auto task = new Task();
                            if (value.isObject()) {
                                QJsonObject obj = value.toObject();
                                task->setTaskName(obj["task_name"].toString());
                                task->setDescription(obj["description"].toString());
                                //task->setId(obj["task_id"].toInt());
                                 task->setId(obj["task_id"].toVariant().toLongLong());
                                /*foreach (Task *tsk, taskList)
                                {
                                    if(tsk->getId() == obj["task_id"].toInt())
                                    {
                                        tasklist.push_back(tsk);
                                    }
                                }*/
                                 tasklist.push_back(task);
                            }
                        }
                    } else {
                        qDebug() << "Failed to parse JSON string or data is not an array.";
                    }
                }
            }
        }
    }
    return tasklist;
}



