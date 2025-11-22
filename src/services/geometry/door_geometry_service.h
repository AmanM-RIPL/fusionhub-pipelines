#ifndef DOOR_GEOMETRY_SERVICE_H
#define DOOR_GEOMETRY_SERVICE_H

#include <QObject>
#include "common/opengl/classes/opengl_helper.h"

class DoorGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit DoorGeometryService(QObject *parent = nullptr);
    void generateMesh2D(BIMElement* doorElement, Mesh* mesh);
    void generateMesh3D(BIMElement* doorElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // DOOR_GEOMETRY_SERVICE_H
