#include "base_bim_model.h"

BaseBimModel::BaseBimModel(int id, QString type)
    : m_id(id), m_type(type)
{}

int BaseBimModel::id() const
{
    return m_id;
}

void BaseBimModel::setId(int newId)
{
    m_id = newId;
}

QString BaseBimModel::type() const
{
    return m_type;
}
