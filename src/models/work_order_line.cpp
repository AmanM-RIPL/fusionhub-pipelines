#include "models/work_order_line.h"

WorkOrderLine::WorkOrderLine(int id, const QString& globalId, bool approvalStatus, int workOrderId, const QString& description, const QString& descriptionLine, int taskId,
                                     double amount, double taxAmount, double taxWithHolding, double retentionAmount, QObject* parent)
    : QObject(parent),
    id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    workOrderId(workOrderId),
    description(description),
    descriptionLine(descriptionLine),
    taskId(taskId),
    amount(amount),
    taxAmount(taxAmount),
    taxWithHolding(taxWithHolding),
    retentionAmount(retentionAmount)
{
}

