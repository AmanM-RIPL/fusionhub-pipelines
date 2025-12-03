#include "purchase_order_line_controller.h"

#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

PurchaseOrderLineController::PurchaseOrderLineController(QObject *parent)
    : QObject{parent},
    m_purchaseOrderRepository(RepositoryLocator::instance().purchaseOrderRepository()),
    m_purchaseOrderLineRepository(RepositoryLocator::instance().purchaseOrderLineRepository()),
    m_materialRepository(RepositoryLocator::instance().materialRepository()),
    m_unitOfMeasurementRepository(RepositoryLocator::instance().unitOfMeasurementRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void PurchaseOrderLineController::create(const int &purchaseOrderId, const int &unitOfMeasurementId,
                                         const int &materialId, const int amount,const int quantity, const double tax_amount, const double tax_withholding ) const
{

    QJsonObject jsonObject;
    jsonObject["purchaseOrderId"] = purchaseOrderId;
    jsonObject["materialId"] = materialId;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "PurchaseOrder:EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("PurchaseOrder");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


std::vector<PurchaseOrderLine*> PurchaseOrderLineController::getPurchaseOrderLineList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_purchaseOrderLineRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("PurchaseOrderLine");
        std::vector<PurchaseOrderLine*> purchaseOrderLines;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto purchaseOrderLine = new PurchaseOrderLine();
                QJsonObject jsonObj = jsonDoc.object();
                purchaseOrderLine->setId(i + 1);
                purchaseOrderLine->setGlobalId("123");
                purchaseOrderLine->setApprovalStatus(true);

                purchaseOrderLine->setUnitOfMeasurementId(jsonObj["unitOfMeasurementId"].toInt());

                purchaseOrderLines.push_back(purchaseOrderLine);
            }
        }
        return purchaseOrderLines;
    }
}

std::vector<UnitOfMeasurement*> PurchaseOrderLineController::getUOMList() const
{
    return m_unitOfMeasurementRepository->findAllQML();
}

std::vector<Material*> PurchaseOrderLineController::getMaterialList() const
{
    return m_materialRepository->findAllQML();
}

std::vector<PurchaseOrder*> PurchaseOrderLineController::getPurchaseOrderList() const
{
    return m_purchaseOrderRepository->findAllQML();
}

