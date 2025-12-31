#ifndef COLUMN_GEOMETRY_SERVICE_H
#define COLUMN_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"

class ColumnGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit ColumnGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* columnElement, Mesh* mesh);
    void generateMesh3D(BIMElement* columnElement, Mesh* mesh);
    void updateGeometry(BIMElement* columnElement, const QVector3D& point);

signals:

private:
    OpenglHelper m_openglHelper;    
};

#endif // COLUMN_GEOMETRY_SERVICE_H
