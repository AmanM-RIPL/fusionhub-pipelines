#ifndef SLAB_GEOMETRY_SERVICE_H
#define SLAB_GEOMETRY_SERVICE_H

#include "common/opengl/classes/opengl_helper.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"
#include "common/helper_point.h"
#include "common/edit_option.h"

#include <QJsonObject>

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
#include "transf.hxx"
#include "coverapi.hxx"
#include "transfrm.hxx"

// for faceter
#include "af_api.hxx"
#include "fct_utl.hxx"
#include "af_serializable_mesh.hxx"

class SlabGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit SlabGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* slabElement, Mesh* mesh);
    void generateMesh3D(BIMElement* slabElement, Mesh* mesh);
    void updateGeometry(BIMElement* slabElement, const QVector3D& point, EditOption *editOption);
    void generateWIPMesh2D(BIMElement* slabElement, Mesh* mesh, const QVector3D &point, const Point& screen_point, View *view, QList<HelperPoint> &helperPoints, EditOption *editOption);
    Point updatePoint2D(BIMElement* slabElement, const QList<HelperPoint>& helperPoints, const Point& screen_point, View *view); // value comes from m_middlePointValue
    void generateHelperPoints(BIMElement *bimElement, QList<HelperPoint> &helperPoints);
    void getRayHitPoint(BIMElement* bimElement, View* view, const Point& screen_point, QVector3D &point);

signals:

private:
    OpenglHelper m_openglHelper;

    void generateSlab2D(BODY* &final_body, ENTITY_LIST &ents, std::vector<ReferenceLineSegment> &referenceLine);
    void generateSlab3D(BODY* &final_body, ENTITY_LIST &ents, std::vector<ReferenceLineSegment> &referenceLine, float height, float distance);
};

#endif // SLAB_GEOMETRY_SERVICE_H
