#ifndef PURCHASE_ORDER_CONTROLLER_H
#define PURCHASE_ORDER_CONTROLLER_H

#include <QObject>
#include "repositories/purchase_order_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/material_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/draft_entity_repository.h"

class PurchaseOrderController: public QObject
{
    Q_OBJECT
public:
    explicit PurchaseOrderController(QObject *parent = nullptr);
    Q_INVOKABLE void create( const int &VendorId, const int &MaterialId, const int &UnitOfMeasurementId) const;
    Q_INVOKABLE std::vector<PurchaseOrder*> getPurchaseOrderList(bool isApproved = false) const;

    Q_INVOKABLE std::vector<Vendor*> getVendorList() const;
    Q_INVOKABLE std::vector<Material*> getMaterialList() const;
    Q_INVOKABLE std::vector<UnitOfMeasurement*> getUOMList() const;
private:
    PurchaseOrderRepository* m_purchaseOrderRepository;
    VendorRepository* m_vendorRepository;
    MaterialRepository* m_materialRepository;
    UnitOfMeasurementRepository* m_unitOfMeasurementRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // PURCHASE_CONTROLLER_H
