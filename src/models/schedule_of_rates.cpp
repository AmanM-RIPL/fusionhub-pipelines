#include "models/schedule_of_rates.h"

ScheduleOfRates::ScheduleOfRates(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                   const QString& scheduleOfRatesName, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    scheduleOfRatesName(scheduleOfRatesName)
{
}

