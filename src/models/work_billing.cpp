#include "models/work_billing.h"

WorkBilling::WorkBilling(int id, const QString& globalId,  const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                     const QString& workBillingName, const QString& workOrderName, int workOrderId , QObject* parent)
    : QObject(parent), id(id), globalId(globalId),  approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    workBillingName(workBillingName), workOrderName(workOrderName), workOrderId(workOrderId)
{
}

