#include "models/purchase_order.h"

PurchaseOrder::PurchaseOrder(int id, const QString& globalId, bool approvalStatus,int vendorId, QString& vendorName , QString& purchaseOrderList ,int amount ,
                             QString& materialName, int quantity ,int taxAmount ,int taxWithHolding , int materialId, int unitOfMeasurementId, QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    vendorId(vendorId),
    purchaseOrderList(purchaseOrderList),
    vendorName(vendorName),
    materialName(materialName),
    quantity(quantity),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding),
    materialId(materialId),
    unitOfMeasurementId(unitOfMeasurementId)
{
}
