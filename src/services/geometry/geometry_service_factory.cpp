#include "geometry_service_factory.h"


GeometryServiceFactory::GeometryServiceFactory(QObject *parent)
    : QObject{parent}
{

}

void GeometryServiceFactory::generateMesh2D(BaseBimModel *bimElement, Mesh *mesh)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateMesh2D(static_cast<Wall*>(bimElement), mesh);
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     service.generateMesh2D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     service.generateMesh2D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.generateMesh2D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Door")
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     service.generateMesh2D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Window")
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     service.generateMesh2D(bimElement, mesh);
    // }
}

void GeometryServiceFactory::generateMesh3D(BaseBimModel *bimElement, Mesh *mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateMesh3D(static_cast<Wall*>(bimElement), mesh);
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     service.generateMesh3D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     service.generateMesh3D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.generateMesh3D(bimElement, mesh);
    // }
    // else if (bimElement->getType() == "Door")
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     service.generateMesh3D(bimElement, mesh, pIfcDetailController, pIfcGeometryService);
    // }
    // else if (bimElement->getType() == "Window")
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     service.generateMesh3D(bimElement, mesh, pIfcDetailController, pIfcGeometryService);
    // }
}

void GeometryServiceFactory::updateGeometry(const QVector3D& point, EditOption *editOption, BaseBimModel* bimElement, BaseBimModel* hostElement)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.updateGeometry(static_cast<Wall*>(bimElement), point, editOption);
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     service.updateGeometry(bimElement, point);
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     service.updateGeometry(bimElement, point);
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.updateGeometry(bimElement, point, editOption);
    // }
    // else if (bimElement->getType() == "Door" && hostElement != nullptr)
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     service.updateGeometry(bimElement, hostElement, point);
    // }
    // else if (bimElement->getType() == "Window" && hostElement != nullptr)
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     service.updateGeometry(bimElement, hostElement, point);
    // }
}

void GeometryServiceFactory::generateWIPMesh2D(BaseBimModel *bimElement, Mesh *mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption, BaseBimModel* hostElement)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateWIPMesh2D(static_cast<Wall*>(bimElement), mesh, point, screen_point, view, helperPoints, editOption);
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints);
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints);
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.generateWIPMesh2D(bimElement, mesh, point, screen_point, view, helperPoints, editOption);
    // }
    // else if (bimElement->getType() == "Door")
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     service.generateWIPMesh2D(bimElement, hostElement, mesh, point, screen_point, view, helperPoints);
    // }
    // else if (bimElement->getType() == "Window")
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     service.generateWIPMesh2D(bimElement, hostElement, mesh, point, screen_point, view, helperPoints);
    // }
}

Point GeometryServiceFactory::updatePoint2D(BaseBimModel *bimElement, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view, BaseBimModel* hostElement)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        Point new_point = service.updatePoint2D(static_cast<Wall*>(bimElement), helperPoints, screen_point, view);

        return new_point;
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

    //     return new_point;
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

    //     return new_point;
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     Point new_point = service.updatePoint2D(bimElement, helperPoints, screen_point, view);

    //     return new_point;
    // }
    // else if (bimElement->getType() == "Door")
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     Point new_point = service.updatePoint2D(bimElement, hostElement, helperPoints, screen_point, view);

    //     return new_point;
    // }
    // else if (bimElement->getType() == "Window")
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     Point new_point = service.updatePoint2D(bimElement, hostElement, helperPoints, screen_point, view);

    //     return new_point;
    // }

    return {0.0f, 0.0f};
}

void GeometryServiceFactory::generateHelperPoints(BaseBimModel *bimElement, QList<HelperPoint> &helperPoints)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.generateHelperPoints(static_cast<Wall*>(bimElement), helperPoints);
    }
    // else if (bimElement->getType() == "Beam")
    // {
    //     BeamGeometryService service = BeamGeometryService();
    //     service.generateHelperPoints(bimElement, helperPoints);
    // }
    // else if (bimElement->getType() == "Column")
    // {
    //     ColumnGeometryService service = ColumnGeometryService();
    //     service.generateHelperPoints(bimElement, helperPoints);
    // }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.generateHelperPoints(bimElement, helperPoints);
    // }
    // else if (bimElement->getType() == "Door")
    // {
    //     DoorGeometryService service = DoorGeometryService();
    //     service.generateHelperPoints(bimElement, helperPoints);
    // }
    // else if (bimElement->getType() == "Window")
    // {
    //     WindowGeometryService service = WindowGeometryService();
    //     service.generateHelperPoints(bimElement, helperPoints);
    // }
}

void GeometryServiceFactory::getRayHitPoint(BaseBimModel *bimElement, View *view, const Point &screen_point, QVector3D &point)
{
    if (bimElement->type() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.getRayHitPoint(static_cast<Wall*>(bimElement), view, screen_point, point);
    }
    // else if (bimElement->getType() == "Slab")
    // {
    //     SlabGeometryService service = SlabGeometryService();
    //     service.getRayHitPoint(bimElement, view, screen_point, point);
    // }
}

BaseBimModel* GeometryServiceFactory::generateBimModel(BIMElement *bimElement)
{
    if (bimElement == nullptr) return nullptr;

    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        BaseBimModel* bimModel = service.generateBimModel(bimElement);

        return bimModel;
    }
}

void GeometryServiceFactory::updateBimElement(BIMElement *bimElement, BaseBimModel *baseBimModel)
{
    if (bimElement->getType() == "Wall")
    {
        WallGeometryService service = WallGeometryService();
        service.updateBimElement(bimElement, static_cast<Wall*>(baseBimModel));
    }
}
