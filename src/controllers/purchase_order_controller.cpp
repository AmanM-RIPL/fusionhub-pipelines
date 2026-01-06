#include "purchase_order_controller.h"
#include "vendor_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

PurchaseOrderController::PurchaseOrderController(QObject *parent)
    : QObject{parent},
    m_purchaseOrderRepository(RepositoryLocator::instance().purchaseOrderRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void PurchaseOrderController::create( const int vendorId) const
{
    // PurchaseOrder workOrde

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // purchaseOrder.setId(id_in_milliseconds);
    // purchaseOrder.setGlobalId("123");
    // purchaseOrder.setApprovalStatus(true);
    // purchaseOrder.setDescription(purchaseOrderName);
    // purchaseOrder.setWorkOrderId(workOrderId);

    // m_purchaseOrderRepository->saveQML(&purchaseOrder);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["vendorId"] = vendorId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "PurchaseOrder::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["purchaseOrderName"] = "Cretaed By User";
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
    if (isApproved)
    {
        VendorController vendorController;
        std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);
        std::vector<PurchaseOrder*> purchaseOrders;

        auto approvedList = m_purchaseOrderRepository->findAllQML();

        foreach (const PurchaseOrder *order, approvedList)
        {
            int vendorId = order->getVendorId();

            QString vendorName = "N/A";
            foreach (const Vendor *vendor, vecVendor)
            {
                if (vendor->getId() == vendorId)
                {
                    vendorName = vendor->getVendorName();
                    break;
                }
            }

            PurchaseOrder *purchaseOrder = new PurchaseOrder();

            purchaseOrder->setId(order->getId());
            purchaseOrder->setVendorId(vendorId);
            purchaseOrder->setVendorName(vendorName);

            purchaseOrders.push_back(purchaseOrder);
        }

        return purchaseOrders;
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("PurchaseOrder");
        std::vector<PurchaseOrder*> purchaseOrders;
        VendorController vendorController;

        std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);

        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            int draftId = draftEntitys[i]->getId();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto purchaseOrder = new PurchaseOrder();
                QJsonObject jsonObj = jsonDoc.object();
                purchaseOrder->setId(draftId);
                purchaseOrder->setGlobalId("123");
                purchaseOrder->setApprovalStatus(true);
                purchaseOrder->setVendorId(jsonObj["vendorId"].toInt());

                int vendorId = purchaseOrder->getVendorId();
                QString vendorName = "N/A";
                foreach (const Vendor *vendor, vecVendor)
                {
                    qDebug()<<"woid:" <<vendor->getId();
                    if (vendor->getId() == vendorId)
                    {
                        vendorName = vendor->getVendorName();
                        break;
                    }
                }
                purchaseOrder->setVendorName(vendorName);
                //  purchaseOrder->setVendorId(jsonObj["vendorId"].toInt());


                purchaseOrders.push_back(purchaseOrder);
            }
        }
        return purchaseOrders;
    }
}
