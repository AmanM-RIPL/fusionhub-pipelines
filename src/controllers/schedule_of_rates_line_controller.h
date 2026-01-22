#ifndef SCHEDULE_OF_RATES_LINE_CONTROLLER_H
#define SCHEDULE_OF_RATES_LINE_CONTROLLER_H


#include <QObject>
#include "repositories/schedule_of_rates_line_repository.h"
#include "repositories/draft_entity_repository.h"

class ScheduleOfRatesLineController: public QObject
{
    Q_OBJECT
public:
    explicit ScheduleOfRatesLineController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const int &scheduleSetupId, const int &scheduleOfRateId, const QVariantList &scheduleOfRatesLine) const;
    Q_INVOKABLE void update(int id, const int &scheduleSetupId, const int &scheduleOfRateId, const QVariantList &scheduleOfRatesLine) const;

    Q_INVOKABLE std::vector<ScheduleOfRatesLine*> getScheduleOfRatesLineList(bool isApproved = false) const;

private:
    ScheduleOfRatesLineRepository* m_scheduleOfRatesLineRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // SCHEDULE_OF_RATES_LINE_CONTROLLER_H
