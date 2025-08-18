#ifndef SCHEDULE_SETUP_REPOSITORY_H
#define SCHEDULE_SETUP_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/schedule_setup.h"
#include <memory>
class ScheduleSetupRepository : public QObject, public AbstractRepository<ScheduleSetup> {
    Q_OBJECT
public:
    explicit ScheduleSetupRepository(QObject* parent = nullptr);

    Q_INVOKABLE std::unique_ptr<ScheduleSetup> findById(int id) override;
    std::vector<std::unique_ptr<ScheduleSetup>> findAll() override;
    Q_INVOKABLE std::vector<ScheduleSetup*> findAllQML();
    bool save(const ScheduleSetup& entity) override;
    Q_INVOKABLE bool saveQML(ScheduleSetup* entity);
    bool update(const ScheduleSetup& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<ScheduleSetup>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<ScheduleSetup> mapFromQuery(const QSqlQuery& query) const override;
    ScheduleSetup* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const ScheduleSetup& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
