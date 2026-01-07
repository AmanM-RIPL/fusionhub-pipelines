#include "material_indent_controller.h"
#include "material_controller.h"
#include "task_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

MaterialIndentController::MaterialIndentController(QObject *parent)
    : QObject{parent},
    m_materialIndentRepository(RepositoryLocator::instance().materialIndentRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void MaterialIndentController::create(const double &quantity, const int &materialId, const int &taskId) const
{
    // MaterialIndent materialIndent;

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // materialIndent.setId(id_in_milliseconds);
    // materialIndent.setGlobalId("123");
    // materialIndent.setApprovalStatus(true);
    // materialIndent.setQuantity(quantity);
    // materialIndent.setMaterialId(materialId);
    // materialIndent.setTaskId(taskId);

    // m_materialIndentRepository->saveQML(&materialIndent);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["quantity"] = quantity;
    jsonObject["materialId"] = materialId;
    jsonObject["taskId"] = taskId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "MaterialIndent::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("MaterialIndent");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

void MaterialIndentController::update(int id, const double &quantity, const int &materialId, const int &taskId) const
{
    // MaterialIndent materialIndent;

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // materialIndent.setId(id_in_milliseconds);
    // materialIndent.setGlobalId("123");
    // materialIndent.setApprovalStatus(true);
    // materialIndent.setQuantity(quantity);
    // materialIndent.setMaterialId(materialId);
    // materialIndent.setTaskId(taskId);

    // m_materialIndentRepository->saveQML(&materialIndent);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["quantity"] = quantity;
    jsonObject["materialId"] = materialId;
    jsonObject["taskId"] = taskId;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "MaterialIndent::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("MaterialIndent");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<MaterialIndent*> MaterialIndentController::getMaterialIndentList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    // Get material and task lists
    MaterialController materialController;
    std::vector<Material*> vecMaterial = materialController.getMaterialList(true);

    TaskController taskController;
    std::vector<Task*> vecTask = taskController.getTaskList(true);

    std::vector<MaterialIndent*> materialIndents;

    if(isApproved)
    {
        auto approvedList = m_materialIndentRepository->findAllQML();

        foreach (const MaterialIndent *indent, approvedList)
        {
            int materialId = indent->getMaterialId();
            int taskId = indent->getTaskId();
            double quantity = indent->getQuantity();

            QString materialName = "N/A";
            QString taskName = "N/A";
            foreach (const Material *material, vecMaterial)
            {
                if (material->getId() == materialId)
                {
                    materialName = material->getMaterialName();
                    break;
                }
            }
            foreach (const Task *task, vecTask)
            {
                if (task->getId() == taskId)
                {
                    taskName = task->getTaskName();
                    break;
                }
            }

            MaterialIndent *materialIndent = new MaterialIndent();
            materialIndent->setId(indent->getId());
            materialIndent->setQuantity(quantity);
            materialIndent->setMaterialId(materialId);
            materialIndent->setTaskId(taskId);
            materialIndent->setMaterialName(materialName);
            materialIndent->setTaskName(taskName);

            materialIndents.push_back(materialIndent);
        }

        return materialIndents;
    }
    else
    {
        std::vector<DraftEntity*> draftEntitys = m_draftEntityRepository->findAllQML("MaterialIndent");

        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString jsonString = draftEntitys[i]->getEntitySchema();
            int draftId = draftEntitys[i]->getId();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto materialIndent = new MaterialIndent();
                QJsonObject jsonObj = jsonDoc.object();

                materialIndent->setId(draftId);
                materialIndent->setGlobalId("123");
                materialIndent->setApprovalStatus(false);
                materialIndent->setQuantity(jsonObj["quantity"].toDouble());
                materialIndent->setMaterialId(jsonObj["materialId"].toInt());
                materialIndent->setTaskId(jsonObj["taskId"].toInt());

                int materialId = materialIndent->getMaterialId();
                int taskId = materialIndent->getTaskId();

                QString materialName = "N/A";
                QString taskName = "N/A";

                foreach (const Material *material, vecMaterial)
                {
                    if (material->getId() == materialId)
                    {
                        materialName = material->getMaterialName();
                        break;
                    }
                }

                foreach (const Task *task, vecTask)
                {
                    if (task->getId() == taskId)
                    {
                        taskName = task->getTaskName();
                        break;
                    }
                }

                materialIndent->setMaterialName(materialName);
                materialIndent->setTaskName(taskName);

                materialIndents.push_back(materialIndent);
            }
        }

        return materialIndents;
    }
}


