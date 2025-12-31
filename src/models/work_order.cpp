#include "models/work_order.h"

WorkOrder::WorkOrder(int id, const QString& globalId, bool approvalStatus,
                                 const QString& workOrderName, const QString& vendorName, int vendorId , QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    workOrderName(workOrderName), vendorName(vendorName), vendorId(vendorId)
{
}

