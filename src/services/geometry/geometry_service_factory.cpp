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
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        service.generateMesh2D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        service.generateMesh2D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.generateMesh2D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Door")
    {
        DoorGeometryService service = DoorGeometryService();
        service.generateMesh2D(bimElement, mesh);
    }
}

void GeometryServiceFactory::generateMesh3D(BIMElement *bimElement, Mesh *mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateMesh3D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        service.generateMesh3D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        service.generateMesh3D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.generateMesh3D(bimElement, mesh);
    }
    else if (bimElement->getType() == "Door")
    {
        DoorGeometryService service = DoorGeometryService();
        service.generateMesh3D(bimElement, mesh, pIfcDetailController, pIfcGeometryService);
    }
}

void GeometryServiceFactory::updateGeometry(BIMElement *bimElement, const QVector3D &point)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.updateGeometry(bimElement, point);
    }
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        service.updateGeometry(bimElement, point);
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        service.updateGeometry(bimElement, point);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.updateGeometry(bimElement, point);
    }
}
