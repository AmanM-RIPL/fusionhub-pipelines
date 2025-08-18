#include "ifcdetailrepository.h"

IFCDetailRepository::IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent)
    : QObject{parent}
{
    m_ifcDetailList = ifcDetailList;
}

QList<IFCDetail*> IFCDetailRepository::getIFCDetails()
{
    return m_ifcDetailList;
}
