#ifndef SCHEDULE_OF_RATES_LINE_REPOSITORY_H
#define SCHEDULE_OF_RATES_LINE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/schedule_of_rates_line.h"
#include <memory>
class ScheduleOfRatesLineRepository : public AbstractRepository<ScheduleOfRatesLine> {
public:
    ScheduleOfRatesLineRepository();
    std::unique_ptr<ScheduleOfRatesLine> findById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRatesLine>> findAll() override;
    bool save(const ScheduleOfRatesLine& entity) override;
    bool update(const ScheduleOfRatesLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRatesLine>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<ScheduleOfRatesLine> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const ScheduleOfRatesLine& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
