#ifndef IFC_WALL_CONTROLLER_H
#define IFC_WALL_CONTROLLER_H

#include <QObject>
#include "repositories/ifc_repositories/ifc_wall_repository.h"

class IFCWallController : public QObject
{
    Q_OBJECT
public:
    explicit IFCWallController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &projectName, const QString &height, const QString &width) const;

signals:

private:
    IFCWallRepository* m_ifcWallRepository;
};

#endif // IFC_WALL_CONTROLLER_H
