#include "models/bill_of_quantity.h"

BillOfQuantity::BillOfQuantity(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                   const QString& description,
                   int scheduleOfRatedId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    description(description),
    scheduleOfRatesId(scheduleOfRatesId)
{
}
