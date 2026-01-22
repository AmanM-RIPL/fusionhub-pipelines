#include "models/bill_of_quantity.h"

BillOfQuantity::BillOfQuantity(int id, const QString& globalId, bool approvalStatus,
                   const QString& description,
                   int scheduleOfRatedId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    description(description),
    scheduleOfRatesId(scheduleOfRatesId)
{
}
