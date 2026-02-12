#include "models/work_order.h"

WorkOrder::WorkOrder(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                                 const QString& workOrderName, const QString& vendorName, int vendorId , QObject* parent)
    : QObject(parent), id(id), globalId(globalId),  approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    workOrderName(workOrderName), vendorName(vendorName), vendorId(vendorId)
{
}

