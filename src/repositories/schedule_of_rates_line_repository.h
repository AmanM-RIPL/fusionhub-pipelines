#ifndef SCHEDULE_OF_RATES_LINE_REPOSITORY_H
#define SCHEDULE_OF_RATES_LINE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/schedule_of_rates_line.h"


#include <QObject>
#include <QSqlQuery>
#include <memory>

class ScheduleOfRatesLineRepository : public QObject, public AbstractRepository<ScheduleOfRatesLine> {

    Q_OBJECT

public:
    explicit ScheduleOfRatesLineRepository(QObject* parent = nullptr);

    std::unique_ptr<ScheduleOfRatesLine> findById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRatesLine>> findAll() override;
    Q_INVOKABLE std::vector<ScheduleOfRatesLine*> findAllQML();

    bool save(const ScheduleOfRatesLine& entity) override;
    Q_INVOKABLE bool saveQML(ScheduleOfRatesLine* entity);

    bool update(const ScheduleOfRatesLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ScheduleOfRatesLine>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

    std::unique_ptr<ScheduleOfRatesLine> mapFromQuery(const QSqlQuery& query) const override;
    ScheduleOfRatesLine* mapFromQueryQML(const QSqlQuery& q, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const ScheduleOfRatesLine& entity) const override;

};
#endif
