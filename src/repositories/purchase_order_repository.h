#ifndef PURCHASE_ORDER_REPOSITORY_H
#define PURCHASE_ORDER_REPOSITORY_H

#include <QObject>
#include <QSqlQuery>
#include <memory>

#include "models/purchase_order.h"
#include "repositories/abstract_repository.h"

class PurchaseOrderRepository : public QObject, public AbstractRepository<PurchaseOrder>
{
    Q_OBJECT
public:
    explicit PurchaseOrderRepository(QObject* parent = nullptr);

    std::unique_ptr<PurchaseOrder> findById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrder>> findAll() override;

    Q_INVOKABLE std::vector<PurchaseOrder*> findAllQML();

    bool save(const PurchaseOrder& entity) override;
    Q_INVOKABLE bool saveQML(PurchaseOrder* entity);

    bool update(const PurchaseOrder& entity) override;
    bool deleteById(int id) override;

    std::vector<std::unique_ptr<PurchaseOrder>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

    std::unique_ptr<PurchaseOrder> mapFromQuery(const QSqlQuery& q) const override;
    PurchaseOrder* mapFromQueryQML(const QSqlQuery& q, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& q, const PurchaseOrder& entity) const override;
};

#endif

