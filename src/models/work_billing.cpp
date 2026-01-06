#include "models/work_billing.h"

WorkBilling::WorkBilling(int id, const QString& globalId, bool approvalStatus,
                     const QString& workBillingName, const QString& workOrderName, int workOrderId , QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    workBillingName(workBillingName), workOrderName(workOrderName), workOrderId(workOrderId)
{
}

