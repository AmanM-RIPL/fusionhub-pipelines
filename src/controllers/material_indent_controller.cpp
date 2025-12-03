#include "material_indent_controller.h"
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
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<MaterialIndent*> MaterialIndentController::getMaterialIndentList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_materialIndentRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("MaterialIndent");
        std::vector<MaterialIndent*> materialIndents;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto materialIndent = new MaterialIndent();
                QJsonObject jsonObj = jsonDoc.object();
                materialIndent->setId(i + 1);
                materialIndent->setGlobalId("123");
                materialIndent->setApprovalStatus(true);

                materialIndent->setQuantity(jsonObj["quantity"].toDouble());
                materialIndent->setMaterialId(jsonObj["materialId"].toInt());
                materialIndent->setTaskId(jsonObj["taskId"].toInt());
                materialIndents.push_back(materialIndent);
            }
        }
        return materialIndents;
    }
}


