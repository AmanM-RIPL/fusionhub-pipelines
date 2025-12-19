#ifndef WORK_BILLING_LINE_REPOSITORY_H
#define WORK_BILLING_LINE_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/work_billing_line.h"
#include <memory>
#include <QObject>

class WorkBillingLineRepository : public QObject, public AbstractRepository<WorkBillingLine>
{
    Q_OBJECT

public:

    explicit WorkBillingLineRepository(QObject* parent = nullptr);

    Q_INVOKABLE std::unique_ptr<WorkBillingLine> findById(int id) override;
    std::vector<std::unique_ptr<WorkBillingLine>> findAll() override;
    Q_INVOKABLE std::vector<WorkBillingLine*> findAllQML();
    bool save(const WorkBillingLine& entity) override;
    Q_INVOKABLE bool saveQML(WorkBillingLine* entity);
    bool update(const WorkBillingLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<WorkBillingLine>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    std::unique_ptr<WorkBillingLine> mapFromQuery(const QSqlQuery& query) const override;
    WorkBillingLine* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const WorkBillingLine& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // WORK_BILLING_LINE_REPOSITORY_H
