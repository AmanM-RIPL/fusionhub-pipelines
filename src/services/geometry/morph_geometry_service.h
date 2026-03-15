#ifndef MORPH_GEOMETRY_SERVICE_H
#define MORPH_GEOMETRY_SERVICE_H

#include <QObject>
#include <QUuid>

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"
#include "common/edit_option.h"
#include "door_geometry_service.h"
#include "window_geometry_service.h"
#include "models/bim_models/base_bim_model.h"
#include "models/bim_models/morph.h"

// ACIS header files
#include "acis.hxx"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include "spatial_license.h"
#include "kernapi.hxx"

#include <boolapi.hxx>
#include "api.hxx"
#include "lists.hxx"
#include "fileinfo.hxx"
#include <cstrapi.hxx>
#include "curdef.hxx"
#include "straight.hxx"
#include "sweepapi.hxx"
#include "swp_opts.hxx"
#include "ofstapi.hxx"
#include "wire_offset_options.hxx"
#include "queryapi.hxx"
#include "raytest.hxx"
#include "rayfire_opts.hxx"

// for faceter
#include "af_api.hxx"
#include "fct_utl.hxx"
#include "af_serializable_mesh.hxx"

extern QString gEnvironmentPath;
extern QString gProjectName;

class MorphGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit MorphGeometryService(QObject *parent = nullptr);

    void generateMesh2D(Morph* morphModel, Mesh* mesh);
    void generateMesh3D(Morph *morphModel, Mesh* mesh);
    void updateGeometry(Morph* morphModel, const QVector3D &point, EditOption *editOption);
    void generateWIPMesh2D(Morph* morphModel, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption);
    Point updatePoint2D(Morph* morphModel, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(Morph *morphModel, QList<HelperPoint> &helperPoints);
    void getRayHitPoint(Morph* morphModel, View* view, const Point& screen_point, QVector3D &point);

    // model update
    Morph* generateBimModel(BIMElement* bimElement);
    void updateBimElement(BIMElement* wallElement, Morph* morphModel);

private:
    OpenglHelper m_openglHelper;

    BODY* generateLineEdge(std::vector<SPAposition>& points, const QVector3D &point);

signals:
};

#endif // MORPH_GEOMETRY_SERVICE_H
