#ifndef BUDGET_HEAD_REPOSITORY_H
#define BUDGET_HEAD_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/budget_head.h"
#include <memory>
class BudgetHeadRepository : public QObject, public AbstractRepository<BudgetHead>
{
    Q_OBJECT
public:
    explicit BudgetHeadRepository(QObject* parent = nullptr);


    Q_INVOKABLE std::unique_ptr<BudgetHead> findById(int id) override;
    std::vector<std::unique_ptr<BudgetHead>> findAll() override;
    Q_INVOKABLE std::vector<BudgetHead*> findAllQML();
    bool save(const BudgetHead& entity) override;
    Q_INVOKABLE bool saveQML(BudgetHead* entity);
    Q_INVOKABLE bool update(const BudgetHead& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BudgetHead>> findByApprovalStatus(bool status) override;
    Q_INVOKABLE std::vector<BudgetHead*> findByApprovalStatusQML(bool status);
protected:
    QString getTableName() const override;
    std::unique_ptr<BudgetHead> mapFromQuery(const QSqlQuery& query) const override;
    BudgetHead* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const BudgetHead& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
