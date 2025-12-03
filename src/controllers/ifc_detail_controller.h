#ifndef IFC_DETAIL_CONTROLLER_H
#define IFC_DETAIL_CONTROLLER_H

#include <QObject>
#include "repositories/ifc_detail_repository.h"
#include "common/repository_locator.h"


class IFCDetailController : public QObject
{
    Q_OBJECT
public:
    explicit IFCDetailController(QObject *parent = nullptr);
    Q_INVOKABLE bool create(const QString& projectName, const IFCDetail& ifcDetail);
    Q_INVOKABLE QList<IFCDetail*> getIFCDetails();
    Q_INVOKABLE QList<IFCDetail*> loadIFC(const QString& ifcFilePath);
    Q_INVOKABLE TreeModel* getTreeModel();
    Q_INVOKABLE QString getIfcFilePath(const QString& projectName = nullptr, const QString& ifcFileName = nullptr);
    Q_INVOKABLE OdIfcModelPtr getModelptrFromLoadedIFC(const QString& ifcFilePath);
    Q_INVOKABLE OdIfcFilePtr getIfcFilePtrFromLoadedIFC(const QString& ifcFilePath);

signals:

private:
    IFCDetailRepository* m_ifcDetailRepository;
};

#endif // IFC_DETAIL_CONTROLLER_H
