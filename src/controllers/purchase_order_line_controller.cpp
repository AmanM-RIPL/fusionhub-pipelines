#include "purchase_order_line_controller.h"
#include "purchase_order_controller.h"
#include "vendor_controller.h"
#include "material_controller.h"
#include "unit_of_measurement_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

PurchaseOrderLineController::PurchaseOrderLineController(QObject *parent)
    : QObject{parent},
    m_purchaseOrderLineRepository(RepositoryLocator::instance().purchaseOrderLineRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void PurchaseOrderLineController::create(const int &vendorId, const QVariantList &purchaseOrderLine) const
{
    QJsonObject jsonObject;
    jsonObject["vendorId"] = vendorId;

    QJsonArray lineArray;
    for (const QVariant &item : purchaseOrderLine) {
        lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    }

    jsonObject["listData"] = lineArray;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
  //  qDebug() << "PurchaseOrder:EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "create";
    jsonObjectChangeHistory["description"] = "Created By User";
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
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

void PurchaseOrderLineController::approvedCreate(int purchaseOrderId,int materialId,double quantity,int unitOfMeasurementId,double dollarValue, double taxAmount,double taxWithholding) const
{
    PurchaseOrderLine purchaseOrderLine;

    purchaseOrderLine.setId(0);
    purchaseOrderLine.setGlobalId("123");
    purchaseOrderLine.setApprovalStatus(true);

    purchaseOrderLine.setPurchaseOrderId(purchaseOrderId);
    purchaseOrderLine.setMaterialId(materialId);
    purchaseOrderLine.setQuantity(quantity);
    purchaseOrderLine.setUnitOfMeasurementId(unitOfMeasurementId);
    purchaseOrderLine.setAmount(dollarValue);
    purchaseOrderLine.setTaxAmount(taxAmount);
    purchaseOrderLine.setTaxWithHolding(taxWithholding);

    m_purchaseOrderLineRepository->saveQML(&purchaseOrderLine);
}


void PurchaseOrderLineController::update(int id, const int &vendorId, const QVariantList &purchaseOrderLine) const
{
    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["vendorId"] = vendorId;

    QJsonArray lineArray;
    for (const QVariant &item : purchaseOrderLine) {
        lineArray.append(QJsonObject::fromVariantMap(item.toMap()));
    }

    jsonObject["listData"] = lineArray;

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
    draftEntity.setEntity("PurchaseOrder");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<PurchaseOrderLine*> PurchaseOrderLineController::getPurchaseOrderLineList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    std::vector<PurchaseOrderLine*> purchaseOrderLines;

    // ---------- Approved Data ----------
    if (isApproved)
    {
        return m_purchaseOrderLineRepository->findAllQML();
    }

    // ---- Fetch Draft PurchaseOrder Data ----
    std::vector<DraftEntity*> draftEntitys = m_draftEntityRepository->findAllQML("PurchaseOrder");

    // Reference data for mapping
    VendorController vendorController;
    MaterialController materialController;
    UnitOfMeasurementController unitOfMeasurementController;

    std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);
    std::vector<Material*> vecMaterial = materialController.getMaterialList(true);
    std::vector<UnitOfMeasurement*> vecUnitOfMeasurement = unitOfMeasurementController.getUOMList(true);

    for (int i = 0; i < draftEntitys.size(); i++)
    {
        int draftId = draftEntitys[i]->getId();
        QString jsonString = draftEntitys[i]->getEntitySchema();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

        if (!jsonDoc.isNull() && jsonDoc.isObject())
        {
            QJsonObject mainObject = jsonDoc.object();
            int vendorId = mainObject["vendorId"].toInt();

            QJsonArray lineArray = mainObject["listData"].toArray();
            int lineIndex = 0;

            foreach (const QJsonValue &value, lineArray)
            {
                QJsonObject obj = value.toObject();

                QString materialId           = obj["material_id"].toString();
                QString quantity             = obj["quantity"].toString();
                QString unitOfMeasurementId  = obj["unit_of_measurement_id"].toString();
                QString dollarValue          = obj["dollar_value"].toString();
                QString taxAmount            = obj["tax_amount"].toString();
                QString taxWithholding       = obj["tax_withholding"].toString();

                auto purchaseOrderLine = new PurchaseOrderLine();

                // Assign values
                purchaseOrderLine->setMaterialId(materialId.toInt());
                purchaseOrderLine->setQuantity(quantity.toDouble());
                purchaseOrderLine->setUnitOfMeasurementId(unitOfMeasurementId.toInt());
                purchaseOrderLine->setAmount(dollarValue.toDouble());
                purchaseOrderLine->setTaxAmount(taxAmount.toDouble());
                purchaseOrderLine->setTaxWithHolding(taxWithholding.toDouble());

                // Store IDs
                purchaseOrderLine->setVendorId(vendorId);

                // Draft info
                purchaseOrderLine->setId(draftId);

                // ---- Match Vendor ----
                foreach (const Vendor *v, vecVendor)
                {
                    if (v->getId() == vendorId)
                    {
                        purchaseOrderLine->setVendorName(v->getVendorName());
                        break;
                    }
                }

                // ---- Match Material ----
                foreach (const Material *m, vecMaterial)
                {
                    if (m->getId() == materialId.toInt())
                    {
                        purchaseOrderLine->setMaterialName(m->getMaterialName());
                        break;
                    }
                }

                // ---- Match Unit of Measurement ----
                foreach (const UnitOfMeasurement *uom, vecUnitOfMeasurement)
                {
                    if (uom->getId() == unitOfMeasurementId.toInt())
                    {
                        purchaseOrderLine->setUnitOfMeasurementName(uom->getUomName());
                        break;
                    }
                }

                purchaseOrderLines.push_back(purchaseOrderLine);
                lineIndex++;
            }
        }
    }

    return purchaseOrderLines;
}

std::vector<PurchaseOrderLine*> PurchaseOrderLineController::getPurchaseOrderLineMaterialList(bool isApproved) const
{
    qDebug() << "Fetching Purchase Order Lines with Material Mapping. IsApproved: " << isApproved;

    std::vector<PurchaseOrderLine*> purchaseOrderLines = m_purchaseOrderLineRepository->findAllQML();

    MaterialController materialController;
    std::vector<Material*> vecMaterial = materialController.getMaterialList(true);

    QMap<int, QString> materialMap;
    for (const Material *m : vecMaterial) {
        materialMap.insert(m->getId(), m->getMaterialName());
    }


    for (size_t i = 0; i < purchaseOrderLines.size(); ++i) {
        int mId = purchaseOrderLines[i]->getMaterialId();

        if (materialMap.contains(mId)) {
            purchaseOrderLines[i]->setMaterialName(materialMap.value(mId));
        } else {
            purchaseOrderLines[i]->setMaterialName("Unknown Material");
        }
    }

    return purchaseOrderLines;
}

std::vector<PurchaseOrderLine*> PurchaseOrderLineController::getPurchaseOrderLineVendorList(bool isApproved) const
{
    qDebug() << "Fetching Purchase Order Lines. IsApproved: " << isApproved;

    std::vector<PurchaseOrderLine*> purchaseOrderLines;

    // 1. Repository se saara data uthao (Approved/DB data)
    purchaseOrderLines = m_purchaseOrderLineRepository->findAllQML();

    // 2. Mapping ke liye Reference Controllers
    VendorController vendorController;
    MaterialController materialController;
    PurchaseOrderController poController; // PO ID se Vendor ID nikalne ke liye

    // Reference data collections
    std::vector<Vendor*> vecVendor = vendorController.getVendorList(true);
    std::vector<Material*> vecMaterial = materialController.getMaterialList(true);
    std::vector<PurchaseOrder*> vecPO = poController.getPurchaseOrderList(isApproved);

    // 3. Fast Mapping ke liye Maps (Aapke loop ko speed dene ke liye)
    QMap<int, int> poToVendorMap;
    for (auto po : vecPO) poToVendorMap.insert(po->getId(), po->getVendorId());

    QMap<int, QString> vendorMap;
    for (auto v : vecVendor) vendorMap.insert(v->getId(), v->getVendorName());

    QMap<int, QString> materialMap;
    for (auto m : vecMaterial) materialMap.insert(m->getId(), m->getMaterialName());

    // 4. Mapping Loop (Aapke code ka logic apply karte hue)
    for (int i = 0; i < purchaseOrderLines.size(); i++)
    {
        auto line = purchaseOrderLines[i];

        // ---- Match Vendor (PO -> VendorID -> VendorName) ----
        int poId = line->getPurchaseOrderId();
        if (poToVendorMap.contains(poId))
        {
            int vId = poToVendorMap.value(poId);
            line->setVendorId(vId);
            line->setVendorName(vendorMap.value(vId, "Unknown Vendor"));
        }

        // ---- Match Material Name ----
        int mId = line->getMaterialId();
        if (materialMap.contains(mId))
        {
            line->setMaterialName(materialMap.value(mId));
        }

        // Note: Quantity Repository se pehle hi line object mein aa chuki hogi.
        // Agar zero aa rahi hai, toh Repository ka query check karein.
    }

    return purchaseOrderLines;
}
double PurchaseOrderLineController::getTotalPurchaseExpense() const
{
    auto lines = getPurchaseOrderLineList(true);

    double total = 0;

    for (auto line : lines)
    {
        if (!line) continue;

        total += line->getAmount();
    }

    return total;
}

double PurchaseOrderLineController::getTotalMaterialQuantity() const
{
    auto lines = getPurchaseOrderLineList(true);

    double total = 0;

    for (auto line : lines)
    {
        if (!line) continue;

        total += line->getQuantity();
    }

    return total;
}
QVariantList PurchaseOrderLineController::getMaterialExpenseList() const
{
    QVariantList result;

    MaterialController materialController;

    auto lines = getPurchaseOrderLineList(true);
    auto materials = materialController.getMaterialList(true);

    QMap<int,double> materialExpense;

    for (auto line : lines)
    {
        if (!line) continue;

        materialExpense[line->getMaterialId()] += line->getAmount();
    }

    for (auto mat : materials)
    {
        if (!mat) continue;

        QVariantMap obj;

        obj["materialName"] = mat->getMaterialName();
        obj["amount"] = materialExpense[mat->getId()];

        result.append(obj);
    }

    return result;
}

QVariantList PurchaseOrderLineController::getVendorExpenseList() const
{
    QVariantList result;

    VendorController vendorController;

    auto lines = getPurchaseOrderLineList(true);
    auto vendors = vendorController.getVendorList(true);

    QMap<int,double> vendorExpense;

    for (auto line : lines)
    {
        if (!line) continue;

        vendorExpense[line->getVendorId()] += line->getAmount();
    }

    for (auto v : vendors)
    {
        if (!v) continue;

        QVariantMap obj;

        obj["vendorName"] = v->getVendorName();
        obj["amount"] = vendorExpense[v->getId()];

        result.append(obj);
    }

    return result;
}
