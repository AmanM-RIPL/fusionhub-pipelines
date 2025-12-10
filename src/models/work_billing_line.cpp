#include "models/work_billing_line.h"

WorkBillingLine::WorkBillingLine(int id, const QString& globalId, bool approvalStatus, int workOrderId, int workOrderLineId,
                                 double amount, double taxAmount, double taxWithHolding, double retentionAmount, QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    workOrderId(workOrderId),
    workOrderLineId(workOrderLineId),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding),
    retentionAmount(retentionAmount)
{
}
