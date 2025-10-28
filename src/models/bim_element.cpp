#include "bim_element.h"

BIMElement::BIMElement(int id, const QString &globalId, bool approvalStatus, const QString &type, const QString &name, int level, QObject *parent)
    : QObject(parent)
{
    this->id = id;
    this->globalId = globalId;
    this->approvalStatus = approvalStatus;
    this->type = type;
    this->name = name;
    this->level = level;
}
