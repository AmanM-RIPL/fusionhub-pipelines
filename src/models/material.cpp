#include "models/material.h"

Material::Material(int id, const QString& globalId, bool approvalStatus,
                   const QString& materialName, const QString& category,
                   int unitOfMeasurementId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
      materialName(materialName), category(category),
      unitOfMeasurementId(unitOfMeasurementId)
{
}
