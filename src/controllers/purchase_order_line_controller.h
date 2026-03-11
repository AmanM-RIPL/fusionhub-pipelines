#ifndef PURCHASE_ORDER_LINE_CONTROLLER_H
#define PURCHASE_ORDER_LINE_CONTROLLER_H

#include <QObject>
#include "repositories/purchase_order_line_repository.h"
#include "repositories/draft_entity_repository.h"

class PurchaseOrderLineController: public QObject
{
    Q_OBJECT
public:
    explicit PurchaseOrderLineController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const int &vendorId, const QVariantList &purchaseOrderLine) const;
    Q_INVOKABLE void approvedCreate(int purchaseOrderId,int materialId,double quantity,int unitOfMeasurementId,double dollarValue, double taxAmount,double taxWithholding) const;
    Q_INVOKABLE void update(int id, const int &vendorId, const QVariantList &purchaseOrderLine) const;

    Q_INVOKABLE std::vector<PurchaseOrderLine*> getPurchaseOrderLineList(bool isApproved = false) const;
    Q_INVOKABLE std::vector<PurchaseOrderLine*> getPurchaseOrderLineMaterialList(bool isApproved = false) const;
    Q_INVOKABLE std::vector<PurchaseOrderLine*> getPurchaseOrderLineVendorList(bool isApproved = false) const;

    Q_INVOKABLE double getTotalPurchaseExpense() const;
    Q_INVOKABLE double getTotalMaterialQuantity() const;
    Q_INVOKABLE QVariantList getMaterialExpenseList() const;
    Q_INVOKABLE QVariantList getVendorExpenseList() const;

private:
    PurchaseOrderLineRepository* m_purchaseOrderLineRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // PURCHASE_ORDER_LINE_CONTROLLER_H
