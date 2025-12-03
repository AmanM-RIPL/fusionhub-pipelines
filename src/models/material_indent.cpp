#include "models/material_indent.h"

MaterialIndent::MaterialIndent(int id, const QString& globalId, bool approvalStatus,
                   const double quantity,int materialId,
                   int taskId, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
    quantity(quantity), materialId(materialId),
    taskId(taskId)
{
}
