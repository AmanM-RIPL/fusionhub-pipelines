#include "models/bill_of_quantity_line.h"

BillOfQuantityLine::BillOfQuantityLine(int id, const QString& globalId, bool approvalStatus,
                   const QString& billOfQuantityLineName,
                   int billOfQuantityId, int bimElementId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    billOfQuantityLineName(billOfQuantityLineName), billOfQuantityId(billOfQuantityId), bimElementId(bimElementId)
{

}
