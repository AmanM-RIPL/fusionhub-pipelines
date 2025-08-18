#ifndef PURCHASE_ORDER_LINE_REPOSITORY_H
#define PURCHASE_ORDER_LINE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/purchase_order_line.h"
#include <memory>
class PurchaseOrderLineRepository : public AbstractRepository<PurchaseOrderLine> {
public:
    PurchaseOrderLineRepository();
    std::unique_ptr<PurchaseOrderLine> findById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrderLine>> findAll() override;
    bool save(const PurchaseOrderLine& entity) override;
    bool update(const PurchaseOrderLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrderLine>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<PurchaseOrderLine> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const PurchaseOrderLine& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
