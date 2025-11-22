#include "models/bill_of_quantity_line.h"

BillOfQuantityLine::BillOfQuantityLine(int id, const QString& globalId, bool approvalStatus,
                   const QString& billOfQuantityLineName,
                   int billOfQuantityId, int taskId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    billOfQuantityLineName(billOfQuantityLineName), billOfQuantityId(billOfQuantityId), taskId(taskId)
{

}
