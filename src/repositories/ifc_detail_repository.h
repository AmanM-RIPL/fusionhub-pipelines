#ifndef IFC_DETAIL_REPOSITORY_H
#define IFC_DETAIL_REPOSITORY_H

#include <QObject>
#include "common/ifcdetail.h"
#include "common/tree_model.h"
#include <QList>

#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"
#include "IfcFile.h"
#include "IfcCore.h"
#include "RxObject.h"
#include "QFile"

#include <StaticRxObject.h>
#include <RxDynamicModule.h>
#include <ExPrintConsole.h>
#include <daiHeaderSection.h>
#include <daiHeaderEntities.h>
#include <IfcExamplesCommon.h>
#include "IfcEntity.h"
#include "IfcModel.h"
#include "OdArray.h"


#include "IfcEntityTypes.h"
#include <IfcUtils.h>
#include <IfcBuilder.h>
#include <IfcBuildingBuilder.h>
#include <IfcBuildOptions.h>
#include <IfcCompound.h>
#include <IfcGeomModule.h>
#include <IfcModelContext.h>
#include <IfcEntityTypes.h>
#include "daiUtils/daiUtils.h"
#include "daiRepository.h"

#include "common/ifc_filler_extender.h"
#include "repositories/abstract_repository.h"
#include "common/ifcdetail.h"
#include <memory>

using namespace OdIfc;
using namespace OdIfc4x3_add2;
using namespace OdDAI;

class IFCDetailRepository : public QObject, public AbstractRepository<IFCDetail>
{
    //Q_OBJECT
public:

    explicit IFCDetailRepository(QObject *parent = nullptr);
    //explicit IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent = nullptr);

    QList<IFCDetail*> loadIFC(const QString& ifcFilePath);
    QList<IFCDetail*> getIFCDetails();
    TreeModel* getTreeModel();
    QString getIfcFilePath(const QString& projectName, const QString& ifcFileName);

    std::unique_ptr<IFCDetail> findById(int id){return{};}
    std::vector<std::unique_ptr<IFCDetail>> findAll(){return{};}
    bool save(const IFCDetail& entity);
    bool update(const IFCDetail& entity){return true;}
    bool deleteById(int id){ return true;}
    std::vector<std::unique_ptr<IFCDetail>> findByApprovalStatus(bool status){return {};}
    bool create(const QString& projectName,  const IFCDetail& entity);

    //THeaderEntity* getHeaderFromSection(OdDAI::OdHeaderSectionPtr headerSection);
    void initHeader(OdIfcFile *pDb);
    void fillModel(OdIfcFile *pDb);

protected:
    QString getTableName() const{return "";};
    std::unique_ptr<IFCDetail> mapFromQuery(const QSqlQuery& query) const{return{};}
    void bindEntityToQuery(QSqlQuery& query, const IFCDetail& entity) const{}
    QString getInsertQuery() const{return "";};
    QString getUpdateQuery() const{return "";};
    void collectIfcPoints(OdIfcModelPtr pModel);


signals:

private:
    QList<IFCDetail*> m_ifcDetailList;
    TreeModel* m_treeModel;
    OdArray<OdGePoint3d> getCartesianPoints(OdIfc::OdIfcInstancePtr ifcInstance);
};


template <class THeaderEntity, OdDAI::OdHeaderEntityType headerType>
THeaderEntity*  getHeaderFromSection(OdDAI::OdHeaderSectionPtr headerSection)
{
    OdDAI::ApplicationInstancePtr headerEntity = headerSection->getEntityByType(headerType);
    if (headerEntity.isNull())
    {
        ODA_FAIL_M("Header entity was not created.");
        throw OdError(eNullEntityPointer);
    }

    THeaderEntity* castedPointer = dynamic_cast<THeaderEntity*>(headerEntity.get());

    if (castedPointer == NULL)
    {
        ODA_FAIL_M("Header entity was not created.");
        throw OdError(eIllegalEntityType);
    }

    // Returns valid row pointer. This pointer will be valid until header section destroy.
    return castedPointer;
}


#endif // IFC_DETAIL_REPOSITORY_H
