#include "models/purchase_order.h"

PurchaseOrder::PurchaseOrder(int id, const QString& globalId, bool approvalStatus,
                    int vendorId, int materialId, int unitOfMeasurementId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
     vendorId(vendorId),
     materialId(materialId),
     unitOfMeasurementId(unitOfMeasurementId)
{
}
