#ifndef WORK_ORDER_CONTROLLER_H
#define WORK_ORDER_CONTROLLER_H

#include <QObject>
#include "repositories/work_order_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/draft_entity_repository.h"

class WorkOrderController: public QObject
{
    Q_OBJECT
public:
    explicit WorkOrderController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &name, const int VendorId) const;

    Q_INVOKABLE void approvedCreate(const int vendorId, const QString &workOrderName) const;

    Q_INVOKABLE std::vector<WorkOrder*> getWorkOrderList(bool isApproved = false) const;

 //   Q_INVOKABLE std::vector<Vendor*> getVendorList() const;

private:
    WorkOrderRepository* m_workOrderRepository;
    VendorRepository* m_vendorRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // WORK_ORDER_CONTROLLER_H
