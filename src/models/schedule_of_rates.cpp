#include "models/schedule_of_rates.h"

ScheduleOfRates::ScheduleOfRates(int id, const QString& globalId, bool approvalStatus,
                   const QString& scheduleOfRatesName, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    scheduleOfRatesName(scheduleOfRatesName)
{
}

