#ifndef UNIT_OF_MEASUREMENT_REPOSITORY_H
#define UNIT_OF_MEASUREMENT_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/unit_of_measurement.h"
#include <memory>

class UnitOfMeasurementRepository : public QObject, public AbstractRepository<UnitOfMeasurement>
{
    Q_OBJECT
public:
    explicit UnitOfMeasurementRepository(QObject* parent = nullptr);
    
    Q_INVOKABLE std::unique_ptr<UnitOfMeasurement> findById(int id) override;
    std::vector<std::unique_ptr<UnitOfMeasurement>> findAll() override;
    Q_INVOKABLE std::vector<UnitOfMeasurement*> findAllQML();
    bool save(const UnitOfMeasurement& entity) override;
    Q_INVOKABLE bool saveQML(UnitOfMeasurement* entity);
    bool update(const UnitOfMeasurement& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<UnitOfMeasurement>> findByApprovalStatus(bool status) override;
    
    // Additional methods specific to UnitOfMeasurement
    std::vector<std::unique_ptr<UnitOfMeasurement>> findByUnitType(const QString& unitType);
    std::unique_ptr<UnitOfMeasurement> findByUomName(const QString& uomName);

protected:
    QString getTableName() const override;
    std::unique_ptr<UnitOfMeasurement> mapFromQuery(const QSqlQuery& query) const override;
    UnitOfMeasurement* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const UnitOfMeasurement& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // UNIT_OF_MEASUREMENT_REPOSITORY_H
