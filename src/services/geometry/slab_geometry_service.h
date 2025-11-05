#ifndef SLAB_GEOMETRY_SERVICE_H
#define SLAB_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"

class SlabGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit SlabGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement, Mesh* mesh);
    void generateMesh3D(BIMElement *wallElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;   
};

#endif // SLAB_GEOMETRY_SERVICE_H
