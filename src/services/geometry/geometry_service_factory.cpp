#include "geometry_service_factory.h"


GeometryServiceFactory::GeometryServiceFactory(QObject *parent)
    : QObject{parent}
{

}

void GeometryServiceFactory::generateMesh2D(BIMElement *bimElement, Mesh *mesh)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateMesh2D(bimElement, mesh);
    }
}

void GeometryServiceFactory::generateMesh3D(BIMElement *bimElement, Mesh *mesh)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateMesh3D(bimElement, mesh);
    }
}
