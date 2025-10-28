#include "bim_parameter.h"

BIMParameter::BIMParameter(QObject *parent)
    : QObject{parent}
{}

BIMParameter::BIMParameter(int id, const QString &globalId, bool approvalStatus, const QString &key, const QString &value, int bim_element_id, QObject *parent)
    : QObject(parent)
{
    this->id = id;
    this->globalId = globalId;
    this->approvalStatus = approvalStatus;
    this->key = key;
    this->value = value;
    this->bim_element_id = bim_element_id;
}
