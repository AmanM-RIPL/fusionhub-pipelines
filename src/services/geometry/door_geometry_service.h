#ifndef DOOR_GEOMETRY_SERVICE_H
#define DOOR_GEOMETRY_SERVICE_H

#include <QObject>
#include <QtMath>

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"
#include "ifc_geometry_service.h"

#include "controllers/ifc_detail_controller.h"

class DoorGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit DoorGeometryService(QObject *parent = nullptr);
    void generateMesh2D(BIMElement* doorElement, Mesh* mesh);
    void generateMesh3D(BIMElement* doorElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService);
    FacetModeler::Body generateVoidBody(BIMElement* doorElement, BIMElement* hostElement);
    void updateGeometry(BIMElement *doorElement, BIMElement* hostElement, const QVector3D &point);
    void generateWIPMesh2D(BIMElement* doorElement, BIMElement* hostElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints);
    Point updatePoint2D(BIMElement* doorElement, BIMElement* hostElement, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // DOOR_GEOMETRY_SERVICE_H
