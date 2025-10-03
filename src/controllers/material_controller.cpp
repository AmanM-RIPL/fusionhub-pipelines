#include "material_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

MaterialController::MaterialController(QObject *parent)
    : QObject{parent},
    m_materialRepository(RepositoryLocator::instance().materialRepository()),
    m_unitOfMeasurementRepository(RepositoryLocator::instance().unitOfMeasurementRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void MaterialController::create(const QString &name, const QString &category, const int &unitOfMeasurementId) const
{
    Material material;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    material.setId(id_in_milliseconds);
    material.setGlobalId("123");
    material.setApprovalStatus(true);
    material.setMaterialName(name);
    material.setCategory(category);
    material.setUnitOfMeasurementId(unitOfMeasurementId);

    m_materialRepository->saveQML(&material);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = 0;
    jsonObject["globalId"] = "123";
    jsonObject["approvalStatus"] = true;
    jsonObject["materialName"] = name;
    jsonObject["category"] = category;
    jsonObject["unitOfMeasurementId"] = unitOfMeasurementId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "Material::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getUserId();
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
    draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
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
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto material = new Material();
                QJsonObject jsonObj = jsonDoc.object();
                material->setId(i + 1);
                material->setGlobalId("123");
                material->setApprovalStatus(true);

                material->setMaterialName(jsonObj["materialName"].toString());
                material->setCategory(jsonObj["category"].toString());
                material->setUnitOfMeasurementId(jsonObj["unitOfMeasurementId"].toInt());

                materials.push_back(material);
            }
        }
        return materials;
    }
}

std::vector<UnitOfMeasurement*> MaterialController::getUOMList() const
{
    return m_unitOfMeasurementRepository->findAllQML();
}
