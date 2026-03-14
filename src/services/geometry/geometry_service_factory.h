#ifndef GEOMETRY_SERVICE_FACTORY_H
#define GEOMETRY_SERVICE_FACTORY_H

#include <QObject>

#include "models/bim_element.h"
#include "models/bim_models/base_bim_model.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"
#include "common/edit_option.h"
#include "wall_geometry_service.h"
#include "beam_geometry_service.h"
#include "column_geometry_service.h"
#include "slab_geometry_service.h"
#include "door_geometry_service.h"
#include "window_geometry_service.h"
#include "ifc_geometry_service.h"

#include "controllers/ifc_detail_controller.h"

class GeometryServiceFactory : public QObject
{
    Q_OBJECT
public:
    explicit GeometryServiceFactory(QObject *parent = nullptr);

    static void generateMesh2D(BaseBimModel* bimElement, Mesh* mesh);
    static void generateMesh3D(BaseBimModel* bimElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService);
    static void updateGeometry(const QVector3D& point, EditOption *editOption, BaseBimModel* bimElement, BaseBimModel* hostElement = nullptr);
    static void generateWIPMesh2D(BaseBimModel* bimElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption, BaseBimModel* hostElement = nullptr);
    static Point updatePoint2D(BaseBimModel* bimElement, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view, BaseBimModel* hostElement = nullptr);
    static void generateHelperPoints(BaseBimModel* bimElement, QList<HelperPoint>& helperPoints);
    static void getRayHitPoint(BaseBimModel* bimElement, View* view, const Point& screen_point, QVector3D &point);

    // model management
    static BaseBimModel* generateBimModel(BIMElement* bimElement);
    static void updateBimElement(BIMElement* bimElement, BaseBimModel* baseBimModel);

signals:

};

#endif // GEOMETRY_SERVICE_FACTORY_H
