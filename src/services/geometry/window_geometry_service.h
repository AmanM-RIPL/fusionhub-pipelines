#ifndef WINDOW_GEOMETRY_SERVICE_H
#define WINDOW_GEOMETRY_SERVICE_H

#include <QObject>
#include "common/opengl/classes/opengl_helper.h"

class WindowGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WindowGeometryService(QObject *parent = nullptr);
    void generateMesh2D(BIMElement* windowElement, Mesh* mesh);
    void generateMesh3D(BIMElement* windowElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;
};

#endif // WINDOW_GEOMETRY_SERVICE_H
