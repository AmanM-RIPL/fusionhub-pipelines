#include "models/purchase_order_line.h"

PurchaseOrderLine::PurchaseOrderLine(int id, const QString& globalId, bool approvalStatus,int purchaseOrderId, int vendorId, const QString& vendorName,
                                       int materialId, const QString& materialName ,int amount , int unitOfMeasurementId, const QString& unitOfMeasurementName , double quantity ,double taxAmount ,double taxWithHolding ,double totalIndent, double totalPO, double totalGRN,double totalStoreAmount, double totalPurchaseExpense, QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    purchaseOrderId(purchaseOrderId),
    vendorId(vendorId),
    vendorName(vendorName),
    materialId(materialId),
    materialName(materialName),
    quantity(quantity),
    unitOfMeasurementId(unitOfMeasurementId),
    unitOfMeasurementName(unitOfMeasurementName),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding),
    totalIndent(totalIndent),
    totalPO(totalPO),
    totalGRN(totalGRN),
    totalStoreAmount(totalStoreAmount),
    totalPurchaseExpense(totalPurchaseExpense)
{
}
