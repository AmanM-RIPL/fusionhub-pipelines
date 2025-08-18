#ifndef BILL_OF_QUANTITY_LINE_REPOSITORY_H
#define BILL_OF_QUANTITY_LINE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/bill_of_quantity_line.h"
#include <memory>
class BillOfQuantityLineRepository : public AbstractRepository<BillOfQuantityLine> {
public:
    BillOfQuantityLineRepository();
    std::unique_ptr<BillOfQuantityLine> findById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantityLine>> findAll() override;
    bool save(const BillOfQuantityLine& entity) override;
    bool update(const BillOfQuantityLine& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantityLine>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<BillOfQuantityLine> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const BillOfQuantityLine& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
