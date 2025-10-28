#ifndef IFC_WALL_REPOSITORY_H
#define IFC_WALL_REPOSITORY_H

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

#include "repositories/abstract_repository.h"
#include "common/ifcdetail.h"
#include <memory>


#include "models/ifc_models/ifc_wall.h"

class IFCWallRepository : public QObject, public AbstractRepository<IFCWall>
{
    //Q_OBJECT
public:
    explicit IFCWallRepository(QObject *parent = nullptr);
    QList<IFCWall*> loadIFC(const QString& ifcFilePath);
    QList<IFCWall*> getIFCWalls();
    TreeModel* getTreeModel();

    std::unique_ptr<IFCWall> findById(int id){return{};}
    std::vector<std::unique_ptr<IFCWall>> findAll(){return{};}
    bool save(const IFCWall& entity);
    bool update(const IFCWall& entity){return true;}
    bool deleteById(int id){ return true;}
    std::vector<std::unique_ptr<IFCWall>> findByApprovalStatus(bool status){return {};}

protected:
    QString getTableName() const{return "";};
    std::unique_ptr<IFCWall> mapFromQuery(const QSqlQuery& query) const{return{};}
    void bindEntityToQuery(QSqlQuery& query, const IFCWall& entity) const{}
    QString getInsertQuery() const{return "";};
    QString getUpdateQuery() const{return "";};

signals:
};

#endif // IFC_WALL_REPOSITORY_H
