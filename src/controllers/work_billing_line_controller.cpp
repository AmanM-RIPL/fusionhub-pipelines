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


void WorkBillingLineController::create(const int &workOrderId, const QString &description, const QVariantList &workBillingLine) const
{
    QJsonObject jsonObject;
    jsonObject["workOrderId"] = workOrderId;
    jsonObject["description"] = description;

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
    draftEntity.setEntity("WorkBilling");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

void WorkBillingLineController::approvedCreate(int workOrderLineId, double dollarValue, double taxAmount, double taxWithholdingAmount, double retentionAmount) const
{
    WorkBillingLine workBillingLine;

    workBillingLine.setId(0);
    workBillingLine.setGlobalId("123");
    workBillingLine.setApprovalStatus(true);

    workBillingLine.setWorkOrderLineId(workOrderLineId);
    workBillingLine.setAmount(dollarValue);
    workBillingLine.setTaxAmount(taxAmount);
    workBillingLine.setTaxWithHolding(taxWithholdingAmount);
    workBillingLine.setRetentionAmount(retentionAmount);

    m_workBillingLineRepository->saveQML(&workBillingLine);
}

void WorkBillingLineController::update(int id, const int &workOrderId, const QString &description, const QVariantList &workBillingLine) const
{
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["workOrderId"] = workOrderId;
    jsonObject["description"] = description;
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
    jsonObjectChangeHistory["description"] = "Updated By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate updatedOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(updatedOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("WorkOrder");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}


std::vector<WorkBillingLine*> WorkBillingLineController::getWorkBillingLineList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    std::vector<WorkBillingLine*> workBillingLines;

    // ---------- Approved Data ----------
    if (isApproved)
    {
        return m_workBillingLineRepository->findAllQML();
      //   auto approvedList = m_workBillingLineRepository->findAllQML();
      // qDebug() << "IsApproved: " << approvedList;
      //   for (auto *item : approvedList)
      //   {
      //        qDebug() << "Item: " << item;
      //       workBillingLines.push_back(item);
      //   }
    }

    // ---- Fetch Draft WorkBilling Data ----
    std::vector<DraftEntity*> draftEntitys = m_draftEntityRepository->findAllQML("WorkBilling");

    // Reference data for mapping
    WorkOrderController workOrderController;
    WorkOrderLineController workOrderLineController;

    std::vector<WorkOrder*> vecWorkOrder = workOrderController.getWorkOrderList(true);
    std::vector<WorkOrderLine*> vecWorkOrderLine = workOrderLineController.getWorkOrderLineList(true);

    for (int i = 0; i < draftEntitys.size(); i++)
    {
        int draftId = draftEntitys[i]->getId();
        QString jsonString = draftEntitys[i]->getEntitySchema();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject mainObject = jsonDoc.object();
            int workOrderId = mainObject["workOrderId"].toInt();
            QString description = mainObject["description"].toString();

            QJsonArray lineArray = mainObject["listData"].toArray();
           int lineIndex = 0;
            foreach (const QJsonValue &value, lineArray)
            {
                QJsonObject obj = value.toObject();

                QString dollarValue          = obj["dollar_value"].toString();
                QString taxAmount            = obj["tax_amount"].toString();
                QString taxWithholdingAmount = obj["tax_withholding_amount"].toString();
                QString retentionAmount      = obj["retention_amount"].toString();
                QString workOrderLineId      = obj["work_order_line_id"].toString();

                auto workBillingLine = new WorkBillingLine();

                // Assign values
                workBillingLine->setAmount(dollarValue.toDouble());
                workBillingLine->setTaxAmount(taxAmount.toDouble());
                workBillingLine->setTaxWithHolding(taxWithholdingAmount.toDouble());
                workBillingLine->setRetentionAmount(retentionAmount.toDouble());

                // Store IDs
                workBillingLine->setWorkOrderId(workOrderId);
                workBillingLine->setWorkOrderLineId(workOrderLineId.toInt());

                // Draft info
                workBillingLine->setId(draftId);
                workBillingLine->setWorkBillingName(description);

                // ---- Match Work Order ----
                foreach (const WorkOrder *wo, vecWorkOrder)
                {
                    if (wo->getId() == workOrderId)
                    {
                        workBillingLine->setWorkOrderName(wo->getWorkOrderName());
                        break;
                    }
                }

                // ---- Match Work Order Line ----
                foreach (const WorkOrderLine *wol, vecWorkOrderLine)
                {
                    if (wol->getId() == workOrderLineId.toInt())
                    {
                        workBillingLine->setWorkOrderLineName(wol->getDescriptionLine());
                        break;
                    }
                }

                workBillingLines.push_back(workBillingLine);
                lineIndex++;
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



