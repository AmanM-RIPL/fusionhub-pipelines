#include "ifc_detail_repository.h"

/*
#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"
#include "IfcFile.h"
#include "IfcCore.h"
*/


OdStaticRxObject<MyServices> svcs;

IFCDetailRepository::IFCDetailRepository(QObject* parent) : QObject(parent) {


    odrxInitialize(&svcs);
    odIfcInitialize(true, true);
    odTvInitialize();
}

/*IFCDetailRepository::IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent)
    : QObject{parent}
{
    m_ifcDetailList = ifcDetailList;
}*/

QList<IFCDetail*> IFCDetailRepository::getIFCDetails()
{
    return m_ifcDetailList;
}

QList<IFCDetail*> IFCDetailRepository::loadIFC(const QString& ifcFilePath)
{
    //Here need to populate m_ifcDetailList
    QList<IFCDetail*> ifcDetailList;

    OdString ifcFileName("C:\\Users\\RIPL\\Downloads\\BasicHouse.ifc");

    OdIfcFilePtr pDatabase;
    pDatabase = svcs.createDatabase();
    OdResult res = pDatabase->readFile(ifcFileName);
    qInfo() << "IFCDatabase Loaded: " << (res == tvOk);

    OdIfcModelPtr pIfcModel = pDatabase->getModel();


    return m_ifcDetailList = ifcDetailList;
}
