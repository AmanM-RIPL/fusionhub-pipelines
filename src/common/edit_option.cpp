#include "edit_option.h"

EditOption* EditOption::m_instance = nullptr;

EditOption::EditOption(QObject *parent)
    : QObject{parent}
{
    m_viewType = "ModelView";
    m_editType = "line";
    m_curveTypeVisible = false;
    m_solidTypeVisible = false;
    m_modifyTypeVisible = false;
}

EditOption *EditOption::instance()
{
    if (m_instance == nullptr)
    {
        m_instance  = new EditOption();
    }

    return m_instance;
}

void EditOption::setViewType(QString value)
{
    m_viewType = value;

    emit viewTypeChanged();
}

void EditOption::setEditType(QString value)
{
    m_editType = value;

    emit editTypeChanged();
}

void EditOption::setCurveTypeVisible(bool value)
{
    m_curveTypeVisible = value;

    emit curveTypeVisibleChanged();
}

void EditOption::setSolidTypeVisible(bool value)
{
    m_solidTypeVisible = value;

    emit solidTypeVisibleChanged();
}

void EditOption::setModifyTypeVisible(bool value)
{
    m_modifyTypeVisible = value;

    emit modifyTypeVisibleChanged();
}

