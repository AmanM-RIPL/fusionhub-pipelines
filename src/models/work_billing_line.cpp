#include "models/work_billing_line.h"

WorkBillingLine::WorkBillingLine(int id, const QString& globalId, bool approvalStatus, int workOrderId, int workOrderLineId, const QString& workBillingName,
                                 double amount, double taxAmount, double taxWithHolding, double retentionAmount, QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    workOrderId(workOrderId),
    workOrderLineId(workOrderLineId),
    workBillingName(workBillingName),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding),
    retentionAmount(retentionAmount)
{
}
