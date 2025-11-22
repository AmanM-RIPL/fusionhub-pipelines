#ifndef WORK_ORDER_REPOSITORY_H
#define WORK_ORDER_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/work_order.h"
#include <memory>
class WorkOrderRepository :public QObject, public AbstractRepository<WorkOrder> {
    Q_OBJECT
public:
    explicit WorkOrderRepository(QObject* parent = nullptr);
    std::unique_ptr<WorkOrder> findById(int id) override;
    std::vector<std::unique_ptr<WorkOrder>> findAll() override;
    Q_INVOKABLE std::vector<WorkOrder*> findAllQML();
    bool save(const WorkOrder& entity) override;
    Q_INVOKABLE bool saveQML(WorkOrder* entity);
    bool update(const WorkOrder& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<WorkOrder>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<WorkOrder> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const WorkOrder& entity) const override;
    WorkOrder* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

};
#endif




