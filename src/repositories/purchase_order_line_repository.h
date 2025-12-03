#ifndef PURCHASE_ORDER_LINE_REPOSITORY_H
#define PURCHASE_ORDER_LINE_REPOSITORY_H

#include <QObject>
#include <QSqlQuery>
#include <memory>

#include "models/purchase_order_line.h"
#include "repositories/abstract_repository.h"

class PurchaseOrderLineRepository : public QObject, public AbstractRepository<PurchaseOrderLine>
{
    Q_OBJECT
public:
    explicit PurchaseOrderLineRepository(QObject* parent = nullptr);

    std::unique_ptr<PurchaseOrderLine> findById(int id) override;
    std::vector<std::unique_ptr<PurchaseOrderLine>> findAll() override;

    Q_INVOKABLE std::vector<PurchaseOrderLine*> findAllQML();

    bool save(const PurchaseOrderLine& entity) override;
    Q_INVOKABLE bool saveQML(PurchaseOrderLine* entity);

    bool update(const PurchaseOrderLine& entity) override;
    bool deleteById(int id) override;

    std::vector<std::unique_ptr<PurchaseOrderLine>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

    std::unique_ptr<PurchaseOrderLine> mapFromQuery(const QSqlQuery& q) const override;
    PurchaseOrderLine* mapFromQueryQML(const QSqlQuery& q, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& q, const PurchaseOrderLine& entity) const override;
};

#endif

