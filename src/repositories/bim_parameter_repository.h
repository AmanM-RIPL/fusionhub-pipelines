#ifndef BIM_PARAMETER_REPOSITORY_H
#define BIM_PARAMETER_REPOSITORY_H

#include <QObject>

#include "repositories/abstract_repository.h"
#include "models/bim_parameter.h"

class BIMParameterRepository : public QObject, public AbstractRepository<BIMParameter>
{
    Q_OBJECT
public:
    explicit BIMParameterRepository(QObject *parent = nullptr);
    Q_INVOKABLE std::unique_ptr<BIMParameter> findById(int id) override;
    std::vector<std::unique_ptr<BIMParameter>> findAll() override;
    Q_INVOKABLE std::vector<BIMParameter*> findAllQML();
    bool save(const BIMParameter& entity) override;
    Q_INVOKABLE bool saveQML(BIMParameter* entity);
    bool update(const BIMParameter& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BIMParameter>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<BIMParameter> mapFromQuery(const QSqlQuery& query) const override;
    BIMParameter* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const BIMParameter& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // BIM_PARAMETER_REPOSITORY_H
