#ifndef COLUMN_GEOMETRY_SERVICE_H
#define COLUMN_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"

class ColumnGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit ColumnGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* columnElement, Mesh* mesh);
    void generateMesh3D(BIMElement* columnElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;    
};

#endif // COLUMN_GEOMETRY_SERVICE_H
