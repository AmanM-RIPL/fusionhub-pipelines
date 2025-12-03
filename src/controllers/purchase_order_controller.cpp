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
                auto purchaseOrder = new PurchaseOrder();
                QJsonObject jsonObj = jsonDoc.object();

                purchaseOrder->setId(i + 1);
                purchaseOrder->setGlobalId("123");
                purchaseOrder->setApprovalStatus(true);

                purchaseOrder->setVendorName(jsonObj["vendorName"].toString());

                purchaseOrder->setPurchaseOrderList(jsonObj["purchaseOrder"].toString());

                purchaseOrders.push_back(purchaseOrder);
            }
        }
    }

    // Parse purchaseOrder JSON and extract "rows"

    for (auto po : purchaseOrders)
    {
        QString purchaseOrderJsonString = po->getPurchaseOrderList();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(purchaseOrderJsonString.toUtf8());

        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();

            int rows = jsonObj["rows"].toInt();

            jsonObj["rows"] = rows;
            QString updatedJsonString = QString(QJsonDocument(jsonObj).toJson(QJsonDocument::Compact));
            po->setPurchaseOrderList(updatedJsonString);

            qDebug() << "Updated purchaseOrder:" << updatedJsonString;
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
