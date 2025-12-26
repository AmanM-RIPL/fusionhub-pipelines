#ifndef STAIRS_GEOMETRY_SERVICE_H
#define STAIRS_GEOMETRY_SERVICE_H

#include <QObject>
#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"

class StairsGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit StairsGeometryService(QObject *parent = nullptr);
    void generateMesh2D(BIMElement* stairsElement, Mesh* mesh);
    void generateMesh3D(BIMElement* stairsElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // STAIRS_GEOMETRY_SERVICE_H
