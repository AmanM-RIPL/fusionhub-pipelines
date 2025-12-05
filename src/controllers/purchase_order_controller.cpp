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

void PurchaseOrderController::create(const QString &vendorId,  const QVariant &purchaseOrderData) const
{
    QString purchaseOrder =  CreateJson(purchaseOrderData);
    qDebug() <<"Created purchaseOrder:" << purchaseOrder;

    QJsonObject jsonObject;
    jsonObject["vendorName"] = vendorId;
    jsonObject["purchaseOrder"] = purchaseOrder;

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


std::vector<PurchaseOrder*> PurchaseOrderController::getPurchaseOrderList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    std::vector<PurchaseOrder*> purchaseOrders;

    if (isApproved)
    {
        purchaseOrders = m_purchaseOrderRepository->findAllQML();
    }
    else
    {
        std::vector<DraftEntity*> draftEntitys = m_draftEntityRepository->findAllQML("PurchaseOrder");

        for (int i = 0; i < draftEntitys.size(); i++)
        {
            QString jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                QJsonObject jsonObj = jsonDoc.object();
                QString vendorName = jsonObj["vendorName"].toString();

                QString purchaseOrderString = jsonObj["purchaseOrder"].toString();
                QJsonDocument jsonDocPO = QJsonDocument::fromJson(purchaseOrderString.toUtf8());
                if (!jsonDocPO.isNull() && jsonDocPO.isObject())
                {
                    QJsonArray dataArray = jsonDocPO["data"].toArray();
                    foreach (const QJsonValue & value, dataArray)
                    {
                        auto purchaseOrder = new PurchaseOrder();
                        QJsonObject lineItem = value.toObject();
                        QString vendorId = lineItem["vendor_id"].toString();
                        QString amount = lineItem["amount"].toString();
                        QString materialId = lineItem["material_id"].toString();
                        QString material_name = lineItem["material_name"].toString();
                        QString quantity = lineItem["quantity"].toString();
                        QString tax_amount = lineItem["tax_amount"].toString();
                        QString tax_with_holding = lineItem["tax_with_holding"].toString();
                        QString unit_of_measurementId = lineItem["unit_of_measurement_id"].toString();
                        QString unit_of_measurementName = lineItem["unit_of_measurement_name"].toString();

                        //purchaseOrder->setAmount();
                        purchaseOrder->setVendorName(vendorName);
                        purchaseOrder->setVendorId(vendorId.toInt());
                        purchaseOrder->setAmount(amount.toInt());
                        purchaseOrder->setMaterialId(materialId.toInt());
                        purchaseOrder->setMaterialName(material_name);
                        purchaseOrder->setQuantity(quantity.toInt());
                        purchaseOrder->setTaxAmount(tax_amount.toInt());
                        purchaseOrder->setTaxWithHolding(tax_with_holding.toInt());
                        purchaseOrder->setUnitOfMeasurementId(unit_of_measurementId.toInt());
                        purchaseOrder->setUnitOfMeasurementName(unit_of_measurementName);

                        purchaseOrders.push_back(purchaseOrder);
                    }
                }
            }
        }
    }

    return purchaseOrders;
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

QString PurchaseOrderController::CreateJson(const QVariant &param) const
{
    QJsonObject jsonObject;
    if (param.canConvert<QVariantList>()) {
        QVariantList list = param.toList();
        int len = list.size();
        jsonObject["rows"] = len;

        QJsonArray dataArray;

        for (const QVariant &item : list) {
            qDebug() << "Item:" << item.toString();

            if (item.canConvert<QVariantMap>()) {
                QVariantMap map = item.toMap();
                QJsonObject jsonObjectNew;

                for (auto it = map.begin(); it != map.end(); ++it) {
                    jsonObjectNew.insert(it.key(), QJsonValue::fromVariant(it.value()));
                }

                dataArray.append(jsonObjectNew);
            } else {
                qDebug() << "Item is not a QVariantMap!";
            }
        }

        jsonObject["data"] = dataArray;
        QJsonDocument jsonDoc(jsonObject);
        QString JsonString = jsonDoc.toJson(QJsonDocument::Indented);
        qDebug() <<"Created PurchaseOrder:" << JsonString;

        return JsonString;
    }
}
