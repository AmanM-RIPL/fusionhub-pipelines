#ifndef SLAB_GEOMETRY_SERVICE_H
#define SLAB_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"

class SlabGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit SlabGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* slabElement, Mesh* mesh);
    void generateMesh3D(BIMElement* slabElement, Mesh* mesh);
    void updateGeometry(BIMElement* slabElement, const QVector3D& point);
    void generateWIPMesh2D(BIMElement* slabElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints);
    Point updatePoint2D(BIMElement* slabElement, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints);

signals:

private:
    OpenglHelper m_openglHelper;   
};

#endif // SLAB_GEOMETRY_SERVICE_H
