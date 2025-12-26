#ifndef BEAM_GEOMETRY_SERVICE_H
#define BEAM_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"

class BeamGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit BeamGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* beamElement, Mesh* mesh);
    void generateMesh3D(BIMElement* beamElement, Mesh* mesh);
    void updateGeometry(BIMElement* beamElement, const QVector3D& point);

signals:

private:
    OpenglHelper m_openglHelper;    
};

#endif // BEAM_GEOMETRY_SERVICE_H
