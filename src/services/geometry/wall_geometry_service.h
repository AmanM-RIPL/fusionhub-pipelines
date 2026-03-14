#ifndef WALL_GEOMETRY_SERVICE_H
#define WALL_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"
#include "common/edit_option.h"
#include "door_geometry_service.h"
#include "window_geometry_service.h"
#include "models/bim_models/base_bim_model.h"
#include "models/bim_models/wall.h"

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

class WallGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WallGeometryService(QObject *parent = nullptr);

    void generateMesh2D(Wall* wallModel, Mesh* mesh);
    void generateMesh3D(Wall *wallModel, Mesh* mesh);
    void updateGeometry(Wall* wallModel, const QVector3D &point, EditOption *editOption);
    void generateWIPMesh2D(Wall* wallModel, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption);
    Point updatePoint2D(Wall* wallModel, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(Wall *wallModel, QList<HelperPoint> &helperPoints);
    void getRayHitPoint(Wall* wallModel, View* view, const Point& screen_point, QVector3D &point);

    // model update
    Wall* generateBimModel(BIMElement* bimElement);
    void updateBimElement(BIMElement* wallElement, Wall* wallModel);

signals:

private:
    OpenglHelper m_openglHelper;

    void generateWallLayers2D(std::vector<BODY*> &final_bodies, ENTITY_LIST &ents, std::vector<ReferenceLineSegment> &referenceLine, std::vector<Layer> &layers, float width, QString& referenceLinePosition);
    void generateWallLayers3D(
        std::vector<BODY*> &final_bodies,
        ENTITY_LIST &ents,
        std::vector<ReferenceLineSegment> &referenceLine,
        std::vector<Layer> &layers,
        float width,
        float height,
        QString& referenceLinePosition,
        float slantAngle,
        float taperAngle
    );
};

#endif // WALL_GEOMETRY_SERVICE_H
