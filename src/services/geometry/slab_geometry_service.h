#ifndef SLAB_GEOMETRY_SERVICE_H
#define SLAB_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"

class SlabGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit SlabGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* slabElement, Mesh* mesh);
    void generateMesh3D(BIMElement* slabElement, Mesh* mesh);
    void updateGeometry(BIMElement* slabElement, const QVector3D& point);

signals:

private:
    OpenglHelper m_openglHelper;   
};

#endif // SLAB_GEOMETRY_SERVICE_H
