#ifndef GEOMETRY_SERVICE_FACTORY_H
#define GEOMETRY_SERVICE_FACTORY_H

#include <QObject>

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/view.h"
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

    static void generateMesh2D(BIMElement* bimElement, Mesh* mesh);
    static void generateMesh3D(BIMElement* bimElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService);
    static void updateGeometry(const QVector3D& point, BIMElement* bimElement, BIMElement* hostElement = nullptr);
    static Point generateWIPMesh2D(BIMElement* bimElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, float &length);
    static Point updatePoint2D(BIMElement* bimElement, const float& value, const Point& screen_point, View *view);

signals:

};

#endif // GEOMETRY_SERVICE_FACTORY_H
