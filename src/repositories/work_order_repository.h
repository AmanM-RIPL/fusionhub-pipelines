#ifndef WORK_ORDER_REPOSITORY_H
#define WORK_ORDER_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/work_order.h"
#include <memory>
class WorkOrderRepository : public AbstractRepository<WorkOrder> {
public:
    WorkOrderRepository();
    std::unique_ptr<WorkOrder> findById(int id) override;
    std::vector<std::unique_ptr<WorkOrder>> findAll() override;
    bool save(const WorkOrder& entity) override;
    bool update(const WorkOrder& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<WorkOrder>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<WorkOrder> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const WorkOrder& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
