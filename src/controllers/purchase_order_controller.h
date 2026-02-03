#ifndef PURCHASE_ORDER_CONTROLLER_H
#define PURCHASE_ORDER_CONTROLLER_H

#include <QObject>
#include "repositories/purchase_order_repository.h"
#include "repositories/draft_entity_repository.h"

class PurchaseOrderController: public QObject
{
    Q_OBJECT
public:
    explicit PurchaseOrderController(QObject *parent = nullptr);
    Q_INVOKABLE void create(int vendorId) const;
    Q_INVOKABLE void approvedCreate(int vendorId) const;
    Q_INVOKABLE std::vector<PurchaseOrder*> getPurchaseOrderList(bool isApproved = false) const;

private:
    PurchaseOrderRepository* m_purchaseOrderRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // PURCHASE_CONTROLLER_H
