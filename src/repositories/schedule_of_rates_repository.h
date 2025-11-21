#ifndef SCHEDULE_OF_RATES_REPOSITORY_H
#define SCHEDULE_OF_RATES_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/schedule_of_rates.h"
#include <memory>
class ScheduleOfRatesRepository :public QObject, public AbstractRepository<ScheduleOfRates> {
    Q_OBJECT
public:    
    explicit ScheduleOfRatesRepository(QObject* parent = nullptr);
    std::unique_ptr<ScheduleOfRates> findById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRates>> findAll() override;
    Q_INVOKABLE std::vector<ScheduleOfRates*> findAllQML();
    bool save(const ScheduleOfRates& entity) override;
    Q_INVOKABLE bool saveQML(ScheduleOfRates* entity);
    bool update(const ScheduleOfRates& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRates>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<ScheduleOfRates> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const ScheduleOfRates& entity) const override;
    ScheduleOfRates* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

};
#endif
