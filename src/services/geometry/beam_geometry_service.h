#ifndef BEAM_GEOMETRY_SERVICE_H
#define BEAM_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"

class BeamGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit BeamGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* beamElement, Mesh* mesh);
    void generateMesh3D(BIMElement* beamElement, Mesh* mesh);
    void updateGeometry(BIMElement* beamElement, const QVector3D& point);
    void generateWIPMesh2D(BIMElement* beamElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints);
    Point updatePoint2D(BIMElement* beamElement, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints);

signals:

private:
    OpenglHelper m_openglHelper;    
};

#endif // BEAM_GEOMETRY_SERVICE_H
