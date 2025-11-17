#ifndef WALL_GEOMETRY_SERVICE_H
#define WALL_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"

class WallGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WallGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement, Mesh* mesh);
    void generateMesh3D(BIMElement* wallElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;   
};

#endif // WALL_GEOMETRY_SERVICE_H
