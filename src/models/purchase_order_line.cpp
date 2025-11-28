#include "models/purchase_order_line.h"

PurchaseOrderLine::PurchaseOrderLine(int id, const QString& globalId, bool approvalStatus,int purchaseOrderId, int materialId,
                                      int amount , int unitOfMeasurementId, double quantity ,double taxAmount ,double taxWithHolding , QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    purchaseOrderId(purchaseOrderId),
    materialId(materialId),
    quantity(quantity),
    unitOfMeasurementId(unitOfMeasurementId),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding)
{
}
