#ifndef BILL_OF_QUANTITY_REPOSITORY_H
#define BILL_OF_QUANTITY_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/bill_of_quantity.h"
#include <memory>
class BillOfQuantityRepository :public QObject, public AbstractRepository<BillOfQuantity> {
    Q_OBJECT
public:
    explicit BillOfQuantityRepository(QObject* parent = nullptr);
    std::unique_ptr<BillOfQuantity> findById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantity>> findAll() override;
    Q_INVOKABLE std::vector<BillOfQuantity*> findAllQML();
    Q_INVOKABLE bool saveQML(BillOfQuantity* entity);
    bool save(const BillOfQuantity& entity) override;
    bool update(const BillOfQuantity& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BillOfQuantity>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<BillOfQuantity> mapFromQuery(const QSqlQuery& query) const override;
    BillOfQuantity* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const BillOfQuantity& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
