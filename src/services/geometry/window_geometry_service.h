#ifndef WINDOW_GEOMETRY_SERVICE_H
#define WINDOW_GEOMETRY_SERVICE_H

#include <QObject>
#include <QtMath>

#include "common/opengl/classes/opengl_helper.h"
#include "ifc_geometry_service.h"

#include "controllers/ifc_detail_controller.h"

class WindowGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WindowGeometryService(QObject *parent = nullptr);
    void generateMesh2D(BIMElement* windowElement, Mesh* mesh);
    void generateMesh3D(BIMElement* windowElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService);
    FacetModeler::Body generateVoidBody(BIMElement* doorElement, BIMElement* hostElement);
    void updateGeometry(BIMElement *doorElement, BIMElement* hostElement, const QVector3D &point);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // WINDOW_GEOMETRY_SERVICE_H
