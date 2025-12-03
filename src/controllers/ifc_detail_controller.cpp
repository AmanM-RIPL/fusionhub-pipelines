#include "ifc_detail_controller.h"

extern QString gProjectName;

IFCDetailController::IFCDetailController(QObject *parent)
    : QObject{parent},
      m_ifcDetailRepository(RepositoryLocator::instance().ifcDetailRepository())
{}

QList<IFCDetail*> IFCDetailController::getIFCDetails()
{
    return m_ifcDetailRepository->getIFCDetails();
}

QList<IFCDetail*> IFCDetailController::loadIFC(const QString& ifcFilePath)
{
    return m_ifcDetailRepository->loadIFC(ifcFilePath);
}

OdIfcModelPtr IFCDetailController::getModelptrFromLoadedIFC(const QString& ifcFilePath)
{
    return m_ifcDetailRepository->getModelptrFromLoadedIFC(ifcFilePath);
}

OdIfcFilePtr IFCDetailController::getIfcFilePtrFromLoadedIFC(const QString& ifcFilePath)
{
    return m_ifcDetailRepository->getIfcFilePtrFromLoadedIFC(ifcFilePath);
}

TreeModel* IFCDetailController::getTreeModel()
{
    return m_ifcDetailRepository->getTreeModel();
}

bool IFCDetailController::create(const QString& projectName, const IFCDetail& ifcDetail)
{
     return m_ifcDetailRepository->create(projectName, ifcDetail);
}

QString IFCDetailController::getIfcFilePath(const QString& projectName, const QString& ifcFileName)
{
    //return m_ifcDetailRepository->getIfcFilePath(projectName, ifcFileName);
    if(projectName == nullptr && ifcFileName == nullptr ){
        if(!gProjectName.isEmpty()){
          return m_ifcDetailRepository->getIfcFilePath(gProjectName, gProjectName);
        }
        else{
            return "";
        }
    }
    else{
        return m_ifcDetailRepository->getIfcFilePath(projectName, ifcFileName);
    }

    return "";

}




