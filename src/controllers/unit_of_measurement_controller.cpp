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

    UnitOfMeasurement uom;

    uom.setId(0);
    uom.setGlobalId("123");
    uom.setApprovalStatus(true);
    uom.setConversionToCubicMeter(conversionToCubicMeter);
    uom.setConversionToKilogram(conversionToKilogram);
    uom.setConversionToMeter(conversionToMeter);
    uom.setConversionToSqm(conversionToSqm);
    uom.setUnitType(unitType);
    uom.setUomName(uomName);

    m_unitOfMeasurementRepository->saveQML(&uom);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = 0;
    jsonObject["globalId"] = "123";
    jsonObject["approvalStatus"] = true;
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
    draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory("changeHistory");

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<UnitOfMeasurement*> UnitOfMeasurementController::getUOMList(bool isApproved) const
{
    return m_unitOfMeasurementRepository->findAllQML();
}


