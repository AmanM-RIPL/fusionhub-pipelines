#ifndef SCHEDULE_OF_RATES_CONTROLLER_H
#define SCHEDULE_OF_RATES_CONTROLLER_H

#include <QObject>
#include "repositories/schedule_of_rates_repository.h"
#include "repositories/draft_entity_repository.h"

class ScheduleOfRatesController: public QObject
{
    Q_OBJECT
public:
    explicit ScheduleOfRatesController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &name, const QVariant &costValueParameter) const;
    Q_INVOKABLE std::vector<ScheduleOfRates*> getScheduleOfRatesList(bool isApproved = false) const;

private:
    ScheduleOfRatesRepository* m_scheduleOfRatesRepository;
    DraftEntityRepository* m_draftEntityRepository;

    QString CreateJson(const QVariant &param) const;
};

#endif // SCHEDULE_OF_RATES_CONTROLLER_H
