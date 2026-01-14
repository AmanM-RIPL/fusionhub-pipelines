#ifndef WALL_GEOMETRY_SERVICE_H
#define WALL_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "door_geometry_service.h"
#include "window_geometry_service.h"

class WallGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WallGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement, Mesh* mesh);
    void generateMesh3D(BIMElement *wallElement, Mesh* mesh);
    void updateGeometry(BIMElement* wallElement, const QVector3D &point);
    void generateWIPMesh2D(BIMElement* wallElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view);


signals:

private:
    OpenglHelper m_openglHelper;   
};

#endif // WALL_GEOMETRY_SERVICE_H
