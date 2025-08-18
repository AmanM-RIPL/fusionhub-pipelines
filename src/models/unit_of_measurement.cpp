#include "models/unit_of_measurement.h"

UnitOfMeasurement::UnitOfMeasurement(int id, const QString& globalId, bool approvalStatus,
                                   const QString& uomName, const QString& unitType,
                                   double conversionToSqm, double conversionToCubicMeter,
                                   double conversionToMeter, double conversionToKilogram, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
      uomName(uomName), unitType(unitType), conversionToSqm(conversionToSqm),
      conversionToCubicMeter(conversionToCubicMeter), conversionToMeter(conversionToMeter),
      conversionToKilogram(conversionToKilogram)
{
}
