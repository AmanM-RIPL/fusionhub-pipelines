#ifndef PURCHASE_ORDER_LINE_CONTROLLER_H
#define PURCHASE_ORDER_LINE_CONTROLLER_H

#include <QObject>
#include "repositories/purchase_order_line_repository.h"
#include "repositories/purchase_order_repository.h"
#include "repositories/material_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/draft_entity_repository.h"

class PurchaseOrderLineController: public QObject
{
    Q_OBJECT
public:
    explicit PurchaseOrderLineController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const int &purchaseOrderId, const int &unitOfMeasurementId,
                            const int &materialId, const int amount,const int quantity, const double tax_amount, const double tax_withholding ) const;


    Q_INVOKABLE std::vector<PurchaseOrderLine*> getPurchaseOrderLineList(bool isApproved = false) const;

    Q_INVOKABLE std::vector<PurchaseOrder*> getPurchaseOrderList() const;
    Q_INVOKABLE std::vector<Material*> getMaterialList() const;
    Q_INVOKABLE std::vector<UnitOfMeasurement*> getUOMList() const;
private:
    PurchaseOrderLineRepository* m_purchaseOrderLineRepository;
    PurchaseOrderRepository* m_purchaseOrderRepository;
    MaterialRepository* m_materialRepository;
    UnitOfMeasurementRepository* m_unitOfMeasurementRepository;
    DraftEntityRepository* m_draftEntityRepository;
   // QString CreateJson(const QVariant &param) const;
};

#endif // PURCHASE_ORDER_LINE_CONTROLLER_H
