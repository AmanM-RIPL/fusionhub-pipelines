#include "models/bill_of_quantity.h"

BillOfQuantity::BillOfQuantity(int id, const QString& globalId, bool approvalStatus,
                   const QString& description,
                   int scheduleId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    description(description),
    scheduleId(scheduleId)
{
}
