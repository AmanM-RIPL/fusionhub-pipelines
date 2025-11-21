#ifndef BILL_OF_QUANTITY_CONTROLLER_H
#define BILL_OF_QUANTITY_CONTROLLER_H

#include <QObject>
#include "repositories/bill_of_quantity_repository.h"
#include "repositories/schedule_of_rates_repository.h"
#include "repositories/draft_entity_repository.h"

class BillOfQuantityController: public QObject
{
    Q_OBJECT
public:
    explicit BillOfQuantityController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &description , const int &scheduleId) const;
    Q_INVOKABLE std::vector<BillOfQuantity*> getBillOfQuantityList(bool isApproved = false) const;
    Q_INVOKABLE std::vector<ScheduleOfRates*> getScheduleOfRatesList() const;




private:
    BillOfQuantityRepository* m_billOfQuantityRepository;
    ScheduleOfRatesRepository* m_scheduleOfRatesRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // BILL_OF_QUANTITY_CONTROLLER_H
