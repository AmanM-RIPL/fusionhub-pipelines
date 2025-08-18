#ifndef WORK_ORDER_LINE_REPOSITORY_H
#define WORK_ORDER_LINE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/work_order_line.h"
#include <memory>
class WorkOrderLineRepository : public AbstractRepository<WorkOrderLine> {
public:
    WorkOrderLineRepository();
    std::unique_ptr<WorkOrderLine> findById(int id) override;
    std::vector<std::unique_ptr<WorkOrderLine>> findAll() override;
    bool save(const WorkOrderLine& entity) override;
    bool update(const WorkOrderLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<WorkOrderLine>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<WorkOrderLine> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const WorkOrderLine& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
