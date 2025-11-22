#include "task_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

TaskController::TaskController(QObject *parent)
    : QObject{parent},
    m_taskRepository(RepositoryLocator::instance().taskRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void TaskController::create(const QString &name, const QString &description, const QString &bimElement, const QString &startDate , const QString &endDate ) const
{
    Task task;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    task.setId(id_in_milliseconds);
    task.setGlobalId("123");
    task.setApprovalStatus(true);
    task.setTaskName(name);
    task.setDescription(description);
    task.setBimElement(bimElement);
    task.setStartDate(startDate);
    task.setEndDate(endDate);

    m_taskRepository->saveQML(&task);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["taskName"] = name;
    jsonObject["description"] = description;
    jsonObject["bimElement"] = bimElement;
    jsonObject["startDate"] = startDate;
    jsonObject["endDate"] = endDate;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "Task::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("Task");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<Task*> TaskController::getTaskList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_taskRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("Task");
        std::vector<Task*> tasks;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto task = new Task();
                QJsonObject jsonObj = jsonDoc.object();
                task->setId(i + 1);
                task->setGlobalId("123");
                task->setApprovalStatus(true);
                task->setTaskName(jsonObj["taskName"].toString());
                task->setDescription(jsonObj["description"].toString());
                task->setBimElement(jsonObj["billElement"].toString());
                task->setStartDate(jsonObj["startDate"].toString());
                task->setEndDate(jsonObj["endDate"].toString());
                tasks.push_back(task);
            }
        }
        return tasks;
    }
}
