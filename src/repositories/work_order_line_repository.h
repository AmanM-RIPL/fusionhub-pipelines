#ifndef WORK_ORDER_LINE_REPOSITORY_H
#define WORK_ORDER_LINE_REPOSITORY_H

#include <QObject>
#include <QSqlQuery>
#include <memory>

#include "models/work_order_line.h"
#include "repositories/abstract_repository.h"

class WorkOrderLineRepository : public QObject, public AbstractRepository<WorkOrderLine>
{
    Q_OBJECT
public:
    explicit WorkOrderLineRepository(QObject* parent = nullptr);

    std::unique_ptr<WorkOrderLine> findById(int id) override;
    std::vector<std::unique_ptr<WorkOrderLine>> findAll() override;

    Q_INVOKABLE std::vector<WorkOrderLine*> findAllQML();

    bool save(const WorkOrderLine& entity) override;
    Q_INVOKABLE bool saveQML(WorkOrderLine* entity);

    bool update(const WorkOrderLine& entity) override;
    bool deleteById(int id) override;

    std::vector<std::unique_ptr<WorkOrderLine>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

    std::unique_ptr<WorkOrderLine> mapFromQuery(const QSqlQuery& q) const override;
    WorkOrderLine* mapFromQueryQML(const QSqlQuery& q, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& q, const WorkOrderLine& entity) const override;
};

#endif


