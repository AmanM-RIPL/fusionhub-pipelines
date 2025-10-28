#ifndef IFC_DETAIL_CONTROLLER_H
#define IFC_DETAIL_CONTROLLER_H

#include <QObject>
#include "repositories/ifc_detail_repository.h"

class IFCDetailController : public QObject
{
    Q_OBJECT
public:
    explicit IFCDetailController(QObject *parent = nullptr);
    Q_INVOKABLE QList<IFCDetail*> getIFCDetails();
    Q_INVOKABLE QList<IFCDetail*> loadIFC(const QString& ifcFilePath);

signals:

private:
    IFCDetailRepository* m_ifcDetailRepository;
};

#endif // IFC_DETAIL_CONTROLLER_H
