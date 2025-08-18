#ifndef PURCHASE_ORDER_REPOSITORY_H
#define PURCHASE_ORDER_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/purchase_order.h"
#include <memory>
class PurchaseOrderRepository : public AbstractRepository<PurchaseOrder> {
public:
    PurchaseOrderRepository();
    std::unique_ptr<PurchaseOrder> findById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrder>> findAll() override;
    bool save(const PurchaseOrder& entity) override;
    bool update(const PurchaseOrder& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrder>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<PurchaseOrder> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const PurchaseOrder& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
