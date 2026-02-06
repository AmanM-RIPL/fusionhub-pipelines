#include "material_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

MaterialController::MaterialController(QObject *parent)
    : QObject{parent},
    m_materialRepository(RepositoryLocator::instance().materialRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void MaterialController::approvedCreate(const QString &name, const QString &category, const int &unitOfMeasurementId) const
{
    Material material;

   // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
   // material.setId(id_in_milliseconds);
    material.setGlobalId("123");
  //  material.setApprovalStatus(true);
    material.setMaterialName(name);
    material.setCategory(category);
    material.setUnitOfMeasurementId(unitOfMeasurementId);

    m_materialRepository->saveQML(&material);

}

void MaterialController::create(const QString &name, const QString &category, const int &unitOfMeasurementId) const
{
    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
   // jsonObject["globalId"] = "";
   // jsonObject["approvalStatus"] = "";
    jsonObject["materialName"] = name;
    jsonObject["category"] = category;
    jsonObject["unitOfMeasurementId"] = unitOfMeasurementId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "Material::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("Material");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


void MaterialController::update(int id, const QString &name, const QString &category, const int &unitOfMeasurementId) const
{
    QJsonObject jsonObject;
    jsonObject["unitOfMeasurementId"] = unitOfMeasurementId;
    jsonObject["materialName"] = name;
    jsonObject["category"] = category;
    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    // qDebug() << "PurchaseOrder:EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "update";
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
    draftEntity.setEntity("Material");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<Material*> MaterialController::getMaterialList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_materialRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("Material");
        std::vector<Material*> materials;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            int draftId = draftEntitys[i]->getId();
            QString approvalStatus = draftEntitys[i]->getApprovalStatus();
            int nextApprovingUser = draftEntitys[i]->getNextApprovingUser();
            int createdByUser = draftEntitys[i]->getCreatedByUser();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto material = new Material();
                QJsonObject jsonObj = jsonDoc.object();
                material->setId(draftId);
                material->setGlobalId("123");
                material->setApprovalStatus(approvalStatus);
                material->setCreatedByUser(createdByUser);
                material->setNextApprovingUser(nextApprovingUser);
                material->setMaterialName(jsonObj["materialName"].toString());
                material->setCategory(jsonObj["category"].toString());
                material->setUnitOfMeasurementId(jsonObj["unitOfMeasurementId"].toInt());

                materials.push_back(material);
            }
        }
        return materials;
    }
}

void MaterialController::approve(int id) const
{
    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
   // draftEntity.setProject(gProjectId);
    draftEntity.setNextApprovingUser(0);
    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "Approve";
    jsonObjectChangeHistory["description"] = "Approved By User";
    jsonObjectChangeHistory["approvalHistory"] = "Approved";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate approvedOn = QDate::currentDate();

    draftEntity.setChangeHistory(changeHistory);
    draftEntity.setCreatedOn(approvedOn);
    draftEntity.setApprovalStatus("Approved");
   // draftEntity.setApprovedByUser(gUser->getId());

    m_draftEntityRepository->approveQML(&draftEntity);
}

void MaterialController::cancel(int id, const QString &cancellationReason) const
{
    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
    draftEntity.setNextApprovingUser(0);

    // Create change history for cancellation
    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "cancel";
    jsonObjectChangeHistory["approvalHistory"] = cancellationReason;

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate cancelledOn = QDate::currentDate();

    draftEntity.setChangeHistory(changeHistory);
    draftEntity.setCreatedOn(cancelledOn);
    draftEntity.setApprovalStatus("cancelled");  // Update status to cancelled
   // draftEntity.setCancelledByUser(gUser->getId());

    m_draftEntityRepository->cancelQML(&draftEntity);
}
