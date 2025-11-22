#include "purchase_order_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

PurchaseOrderController::PurchaseOrderController(QObject *parent)
    : QObject{parent},
    m_purchaseOrderRepository(RepositoryLocator::instance().purchaseOrderRepository()),
    m_vendorRepository(RepositoryLocator::instance().vendorRepository()),
    m_materialRepository(RepositoryLocator::instance().materialRepository()),
    m_unitOfMeasurementRepository(RepositoryLocator::instance().unitOfMeasurementRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void PurchaseOrderController::create(const int &vendorId,const int &materialId,const int &unitOfMeasurementId) const
{

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["vendorId"] = vendorId;
    jsonObject["materialId"] = materialId;
    jsonObject["unitOfMeasurementId"] = unitOfMeasurementId;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "PurchaseOrder::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
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
    draftEntity.setEntity("PurchaseOrder");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<PurchaseOrder*> PurchaseOrderController::getPurchaseOrderList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_purchaseOrderRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("PurchaseOrder");
        std::vector<PurchaseOrder*> purchaseOrders;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto purchaseOrder = new PurchaseOrder();
                QJsonObject jsonObj = jsonDoc.object();
                purchaseOrder->setId(i + 1);
                purchaseOrder->setGlobalId("123");
                purchaseOrder->setApprovalStatus(true);

                purchaseOrder->setVendorId(jsonObj["vendorId"].toInt());
                purchaseOrder->setMaterialId(jsonObj["materialId"].toInt());
                purchaseOrder->setUnitOfMeasurementId(jsonObj["unitOfMeasurementId"].toInt());

                purchaseOrders.push_back(purchaseOrder);
            }
        }
        return purchaseOrders;
    }
}

std::vector<Vendor*> PurchaseOrderController::getVendorList() const
{
    return m_vendorRepository->findAllQML();
}

std::vector<Material*> PurchaseOrderController::getMaterialList() const
{
    return m_materialRepository->findAllQML();
}

std::vector<UnitOfMeasurement*> PurchaseOrderController::getUOMList() const
{
    return m_unitOfMeasurementRepository->findAllQML();
}
