#ifndef IFCDETAILREPOSITORY_H
#define IFCDETAILREPOSITORY_H

#include <QObject>
#include "ifcdetail.h"
#include <QList>

class IFCDetailRepository : public QObject
{
    Q_OBJECT
public:
    explicit IFCDetailRepository(QList<IFCDetail*> ifcDetailList, QObject *parent = nullptr);

    Q_INVOKABLE QList<IFCDetail*> getIFCDetails();

signals:

private:
    QList<IFCDetail*> m_ifcDetailList;
};

#endif // IFCDETAILREPOSITORY_H
