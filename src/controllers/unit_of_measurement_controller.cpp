#include "unit_of_measurement_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;


UnitOfMeasurementController::UnitOfMeasurementController(QObject *parent)
    : QObject{parent},
    m_unitOfMeasurementRepository(RepositoryLocator::instance().unitOfMeasurementRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void UnitOfMeasurementController::create(const QString& uomName, const QString& unitType,
                                        const double& conversionToSqm, const double& conversionToCubicMeter,
                                        const double& conversionToMeter, const double& conversionToKilogram) const
{

    // UnitOfMeasurement uom;
    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // uom.setId(id_in_milliseconds);
    // uom.setGlobalId("123");
    // uom.setApprovalStatus(true);
    // uom.setConversionToCubicMeter(conversionToCubicMeter);
    // uom.setConversionToKilogram(conversionToKilogram);
    // uom.setConversionToMeter(conversionToMeter);
    // uom.setConversionToSqm(conversionToSqm);
    // uom.setUnitType(unitType);
    // uom.setUomName(uomName);

    // m_unitOfMeasurementRepository->saveQML(&uom);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = true;
    jsonObject["conversionToCubicMeter"] = conversionToCubicMeter;
    jsonObject["conversionToKilogram"] = conversionToKilogram;
    jsonObject["conversionToMeter"] = conversionToMeter;
    jsonObject["conversionToSqm"] = conversionToSqm;
    jsonObject["unitType"] = unitType;
    jsonObject["uomName"] = uomName;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << entitySchema;

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("UnitOfMeasurement");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory("changeHistory");

    m_draftEntityRepository->saveQML(&draftEntity);
}

void UnitOfMeasurementController::update( int id, const QString& uomName, const QString& unitType,
                                         const double& conversionToSqm, const double& conversionToCubicMeter,
                                         const double& conversionToMeter, const double& conversionToKilogram) const
{

    /***********Start of Update DraftEntity  ******************/

    QJsonObject jsonObject;
    jsonObject["id"] = id;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = true;
    jsonObject["conversionToCubicMeter"] = conversionToCubicMeter;
    jsonObject["conversionToKilogram"] = conversionToKilogram;
    jsonObject["conversionToMeter"] = conversionToMeter;
    jsonObject["conversionToSqm"] = conversionToSqm;
    jsonObject["unitType"] = unitType;
    jsonObject["uomName"] = uomName;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << entitySchema;

    QDate updatedOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(updatedOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("UnitOfMeasurement");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory("changeHistory");

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<UnitOfMeasurement*> UnitOfMeasurementController::getUOMList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_unitOfMeasurementRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("UnitOfMeasurement");
        std::vector<UnitOfMeasurement*> unitOfMeasurements;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
           // QString idStr = QString::number(draftEntitys[i]->getId());
            int idValue = draftEntitys[i]->getId();

            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto uom = new UnitOfMeasurement();
                QJsonObject jsonObj = jsonDoc.object();
               // uom->setId(i + 1);
                uom->setId(idValue);
                uom->setGlobalId("123");
                uom->setApprovalStatus(true);
                uom->setUomName(jsonObj["uomName"].toString());
                uom->setUnitType(jsonObj["unitType"].toString());
                uom->setConversionToSqm(jsonObj["conversionToSqm"].toDouble());
                uom->setConversionToMeter(jsonObj["conversionToMeter"].toDouble());
                uom->setConversionToKilogram(jsonObj["conversionToKilogram"].toDouble());
                uom->setConversionToCubicMeter(jsonObj["conversionToCubicMeter"].toDouble());
                unitOfMeasurements.push_back(uom);
            }
        }
        return unitOfMeasurements;
    }
}


