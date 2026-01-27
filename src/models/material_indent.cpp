#include "models/material_indent.h"
#include "models/material.h"
#include "models/task.h"
MaterialIndent::MaterialIndent(int id, const QString& globalId, bool approvalStatus,
                   const double quantity,int materialId,
                   int taskId,const QString& materialName, const QString& taskName, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    quantity(quantity), materialId(materialId),
    taskId(taskId), materialName(materialName), taskName(taskName)
{
}
