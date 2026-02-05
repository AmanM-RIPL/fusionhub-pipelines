#ifndef UNIT_OF_MEASUREMENT_CONTROLLER_H
#define UNIT_OF_MEASUREMENT_CONTROLLER_H

#include <QObject>
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/draft_entity_repository.h"

class UnitOfMeasurementController:  public QObject
{
    Q_OBJECT
public:

    explicit UnitOfMeasurementController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const QString& uomName, const QString& unitType,
                            const double& conversionToSqm, const double& conversionToCubicMeter,
                            const double& conversionToMeter, const double& conversionToKilogram) const;
    Q_INVOKABLE void approvedCreate(const QString& uomName, const QString& unitType,
                                    const double& conversionToSqm, const double& conversionToCubicMeter,
                                    const double& conversionToMeter, const double& conversionToKilogram) const;

    Q_INVOKABLE void update(int id, const QString& uomName, const QString& unitType,
                            const double& conversionToSqm, const double& conversionToCubicMeter,
                            const double& conversionToMeter, const double& conversionToKilogram) const;

    Q_INVOKABLE std::vector<UnitOfMeasurement*> getUOMList(bool isApproved = false) const;

private:
    UnitOfMeasurementRepository* m_unitOfMeasurementRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // UNIT_OF_MEASUREMENT_CONTROLLER_H
