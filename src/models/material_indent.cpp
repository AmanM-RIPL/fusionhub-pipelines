#include "models/material_indent.h"
#include "models/material.h"
#include "models/task.h"
MaterialIndent::MaterialIndent(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser,int createdByUser,
                   const double quantity,int materialId,
                   int taskId,const QString& materialName, const QString& taskName, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),nextApprovingUser(nextApprovingUser),createdByUser(createdByUser),
    quantity(quantity), materialId(materialId),
    taskId(taskId), materialName(materialName), taskName(taskName)
{
}
