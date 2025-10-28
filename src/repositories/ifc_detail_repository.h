#ifndef IFC_DETAIL_REPOSITORY_H
#define IFC_DETAIL_REPOSITORY_H

#include <QObject>
#include "common/ifcdetail.h"
#include "common/myglitem.h"
#include <QList>

#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"
#include "IfcFile.h"
#include "IfcCore.h"
#include "RxObject.h"

#include "repositories/abstract_repository.h"
#include "common/ifcdetail.h"
#include <memory>


class IFCDetailRepository : public QObject, public AbstractRepository<IFCDetail>
{
    //Q_OBJECT
public:

    explicit IFCDetailRepository(QObject *parent = nullptr);
    //explicit IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent = nullptr);

    /*Q_INVOKABLE*/ QList<IFCDetail*> loadIFC(const QString& ifcFilePath);
    /*Q_INVOKABLE*/ QList<IFCDetail*> getIFCDetails();

    std::unique_ptr<IFCDetail> findById(int id){return{};}
    std::vector<std::unique_ptr<IFCDetail>> findAll(){return{};}
    bool save(const IFCDetail& entity){return true;}
    bool update(const IFCDetail& entity){return true;}
    bool deleteById(int id){ return true;}
    std::vector<std::unique_ptr<IFCDetail>> findByApprovalStatus(bool status){return {};}

protected:
    QString getTableName() const{return "";};
    std::unique_ptr<IFCDetail> mapFromQuery(const QSqlQuery& query) const{return{};}
    void bindEntityToQuery(QSqlQuery& query, const IFCDetail& entity) const{}
    QString getInsertQuery() const{return "";};
    QString getUpdateQuery() const{return "";};


signals:

private:
    QList<IFCDetail*> m_ifcDetailList;

};

#endif // IFC_DETAIL_REPOSITORY_H
