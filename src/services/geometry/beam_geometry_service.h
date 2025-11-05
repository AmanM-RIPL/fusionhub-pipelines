#ifndef BEAM_GEOMETRY_SERVICE_H
#define BEAM_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"

class BeamGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit BeamGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement, Mesh* mesh);
    void generateMesh3D(BIMElement *wallElement, Mesh* mesh);

signals:

private:
    OpenglHelper m_openglHelper;    
};

#endif // BEAM_GEOMETRY_SERVICE_H
