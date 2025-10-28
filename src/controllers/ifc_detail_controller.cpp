#include "ifc_detail_controller.h"

IFCDetailController::IFCDetailController(QObject *parent)
    : QObject{parent}
{}

QList<IFCDetail*> IFCDetailController::getIFCDetails()
{
    return m_ifcDetailRepository->getIFCDetails();
}

QList<IFCDetail*> IFCDetailController::loadIFC(const QString& ifcFilePath)
{
    return m_ifcDetailRepository->loadIFC(ifcFilePath);
}
