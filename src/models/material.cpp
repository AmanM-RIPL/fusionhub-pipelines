#include "models/material.h"

Material::Material(int id, const QString& globalId, const QString& approvalStatus,
                   const QString& materialName, const QString& category,
                   int unitOfMeasurementId, int nextApprovingUser,int createdByUser, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
      materialName(materialName), category(category),
    unitOfMeasurementId(unitOfMeasurementId),nextApprovingUser(nextApprovingUser), createdByUser(createdByUser)
{
}
