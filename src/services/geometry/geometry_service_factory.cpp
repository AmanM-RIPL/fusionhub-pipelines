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
    else if (bimElement->getType() == "Window")
    {
        WindowGeometryService service = WindowGeometryService();
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
    else if (bimElement->getType() == "Window")
    {
        WindowGeometryService service = WindowGeometryService();
        service.generateMesh3D(bimElement, mesh, pIfcDetailController, pIfcGeometryService);
    }
}

void GeometryServiceFactory::updateGeometry(const QVector3D& point, BIMElement* bimElement, BIMElement* hostElement)
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
    else if (bimElement->getType() == "Door" && hostElement != nullptr)
    {
        DoorGeometryService service = DoorGeometryService();
        service.updateGeometry(bimElement, hostElement, point);
    }
    else if (bimElement->getType() == "Window" && hostElement != nullptr)
    {
        WindowGeometryService service = WindowGeometryService();
        service.updateGeometry(bimElement, hostElement, point);
    }
}

Point GeometryServiceFactory::generateWIPMesh2D(BIMElement *bimElement, Mesh *mesh, const QVector3D &point, const Point& screen_point, View *view, float &length)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        Point middle_point = service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, length);

        return middle_point;
    }

    return {0.0f, 0.0f};
}

Point GeometryServiceFactory::updatePoint2D(BIMElement *bimElement, const float &value, const Point &screen_point, View *view)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        Point new_point = service.updatePoint2D(bimElement, value, screen_point, view);

        return new_point;
    }

    return {0.0f, 0.0f};
}
