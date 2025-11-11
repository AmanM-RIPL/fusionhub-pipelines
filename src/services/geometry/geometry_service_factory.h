#ifndef GEOMETRY_SERVICE_FACTORY_H
#define GEOMETRY_SERVICE_FACTORY_H

#include <QObject>

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "wall_geometry_service.h"

class GeometryServiceFactory : public QObject
{
    Q_OBJECT
public:
    explicit GeometryServiceFactory(QObject *parent = nullptr);

    static void generateMesh2D(BIMElement* bimElement, Mesh* mesh);
    static void generateMesh3D(BIMElement* bimElement, Mesh* mesh);

signals:

};

#endif // GEOMETRY_SERVICE_FACTORY_H
