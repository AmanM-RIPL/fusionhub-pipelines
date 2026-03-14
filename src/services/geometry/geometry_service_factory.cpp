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

void GeometryServiceFactory::updateGeometry(const QVector3D& point, EditOption *editOption, BIMElement* bimElement, BIMElement* hostElement)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.updateGeometry(bimElement, point, editOption);
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
        service.updateGeometry(bimElement, point, editOption);
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

void GeometryServiceFactory::generateWIPMesh2D(BIMElement *bimElement, Mesh *mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption, BIMElement* hostElement)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints, editOption);
    }
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints);
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints, editOption);
    }
    else if (bimElement->getType() == "Door")
    {
        DoorGeometryService service = DoorGeometryService();
        service.generateWIPMesh2D(bimElement, hostElement, mesh, point, screen_point, view, helperPoints);
    }
    else if (bimElement->getType() == "Window")
    {
        WindowGeometryService service = WindowGeometryService();
        service.generateWIPMesh2D(bimElement, hostElement, mesh, point, screen_point, view, helperPoints);
    }
}

Point GeometryServiceFactory::updatePoint2D(BIMElement *bimElement, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view, BIMElement* hostElement)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

        return new_point;
    }
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

        return new_point;
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

        return new_point;
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

        return new_point;
    }
    else if (bimElement->getType() == "Door")
    {
        DoorGeometryService service = DoorGeometryService();
        Point new_point = service.updatePoint2D(bimElement, hostElement, helperPoints, screen_point, view);

        return new_point;
    }
    else if (bimElement->getType() == "Window")
    {
        WindowGeometryService service = WindowGeometryService();
        Point new_point = service.updatePoint2D(bimElement, hostElement, helperPoints, screen_point, view);

        return new_point;
    }

    return {0.0f, 0.0f};
}

void GeometryServiceFactory::generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
    else if (bimElement->getType() == "Beam")
    {
        BeamGeometryService service = BeamGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
    else if (bimElement->getType() == "Column")
    {
        ColumnGeometryService service = ColumnGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
    else if (bimElement->getType() == "Door")
    {
        DoorGeometryService service = DoorGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
    else if (bimElement->getType() == "Window")
    {
        WindowGeometryService service = WindowGeometryService();
        service.generateHelperPoints(bimElement, helperPoints);
    }
}

void GeometryServiceFactory::getRayHitPoint(BIMElement *bimElement, View *view, const Point &screen_point, QVector3D &point)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.getRayHitPoint(bimElement, view, screen_point, point);
    }
    else if (bimElement->getType() == "Slab")
    {
        SlabGeometryService service = SlabGeometryService();
        service.getRayHitPoint(bimElement, view, screen_point, point);
    }
}
