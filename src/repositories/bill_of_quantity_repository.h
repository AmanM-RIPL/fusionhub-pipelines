#ifndef BILL_OF_QUANTITY_REPOSITORY_H
#define BILL_OF_QUANTITY_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/bill_of_quantity.h"
#include <memory>
class BillOfQuantityRepository : public AbstractRepository<BillOfQuantity> {
public:
    BillOfQuantityRepository();
    std::unique_ptr<BillOfQuantity> findById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantity>> findAll() override;
    bool save(const BillOfQuantity& entity) override;
    bool update(const BillOfQuantity& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantity>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<BillOfQuantity> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const BillOfQuantity& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
