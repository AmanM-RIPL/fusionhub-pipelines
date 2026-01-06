#ifndef WORK_BILLING_REPOSITORY_H
#define WORK_BILLING_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/work_billing.h"
#include <memory>
class WorkBillingRepository :public QObject, public AbstractRepository<WorkBilling> {
    Q_OBJECT
public:
    explicit WorkBillingRepository(QObject* parent = nullptr);
    std::unique_ptr<WorkBilling> findById(int id) override;
    std::vector<std::unique_ptr<WorkBilling>> findAll() override;
    Q_INVOKABLE std::vector<WorkBilling*> findAllQML();
    bool save(const WorkBilling& entity) override;
    Q_INVOKABLE bool saveQML(WorkBilling* entity);
    bool update(const WorkBilling& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<WorkBilling>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<WorkBilling> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const WorkBilling& entity) const override;
    WorkBilling* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

};
#endif




