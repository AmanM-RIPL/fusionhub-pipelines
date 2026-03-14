#ifndef OPENGL_HELPER_H
#define OPENGL_HELPER_H


#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QtMath>

#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <algorithm>

#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "StaticRxObject.h"
#include "FMMdlBody.h"
#include "FMMdlFace.h"
#include "FMMdlSurface.h"
#include "FMMdlEdge.h"
// #include "FMMdlVertex.h"
#include "FMMdlIterators.h"
#include "FMContour2D.h"
#include "FMProfile2D.h"
#include "Ge/GeTrMeshSimplification.h"
#include <IfcModel.h>
//#include <OdIfcCore_dai.h>
#include "Modeler/FMMdlIterators.h"
//#include "OdBrBrep.h"
//#include "OdBrFace.h"

#include "Entities/IfcGeometricRepresentationItem.h"

#include "models/bim_element.h"
#include "common/helper_point.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"
#include "common/opengl/classes/view.h"

// ACIS header files
#include "acis.hxx"
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
#include "faceutil.hxx"
#include "sgquery.hxx"
#include "queryapi.hxx"
#include "raytest.hxx"
#include "rayfire_opts.hxx"

// for faceter
#include "af_api.hxx"
#include "fct_utl.hxx"
#include "af_serializable_mesh.hxx"

using Point = std::array<float, 2>; // (x,y)
using Line = std::array<float, 3>; // (m, b, x) for y = mx + b and x in case m is infinity

struct ReferenceLineSegment
{
    std::vector<Point> points;
    QString type; // line, 3pt-circle, center-circle
};

struct Layer
{
    QString name;
    float width;
};


class OpenglHelper: public QObject
{
    Q_OBJECT
public:
    explicit OpenglHelper(QObject *parent = nullptr);

    void extractBIMParameters(BIMElement *wallElement, std::vector<Point>& referenceLine, float& width, float& height, float& distance);
    void extractBIMParameters(
        BIMElement *wallElement,
        std::vector<ReferenceLineSegment>& referenceLine,
        std::vector<Layer>& layers,
        float& width,
        float& height,
        float& distance,
        float& slantAngle,
        float& taperAngle,
        QString& referenceLinePosition
    );

    void readSATFile(QString& fileName, ENTITY_LIST& ents);
    void saveSATFile(QString& fileName, ENTITY_LIST& ents);

    std::vector<Point> generateParallelCurve(std::vector<Point> referenceCurve, float width);

    // [10,2] where 10 = m and 2 = b in y = mx + b
    Line getParallelLineEquation(Point point1, Point point2, float width);

    // [10, 2] where 10 is x and 2 is y
    Point getIntersectionPoint(Line line1, Line line2);

    // [10, 2] where 10 is x and 2 is y
    Point getParallelProjectionPoint(Point point1, Point point2, float width);

    // used for finding helper lines
    Point getPointAtPerpendicularDistance(Point point1, Point point2, float distance);

    Point getMiddlePoint(Point point1, Point point2);

    Point getPointAtDistance(Point point1, Point point2, float distance); // parallel to the two points

    float getDistanceBetweenPoints(Point point1, Point point2);

    float getAngleBetweenPoints(Point point1, Point point2, Point point3);

    Point getPointAtDistanceAngle(Point point1, Point point2, float angle, float distance); // angle in degrees and counter-clockwise


    // Below methods use ACIS
    void getRayHitPoint(ENTITY_LIST& body_list, View* view, const Point& screen_point, QVector3D &point);

    void convertSPAtransfToQMatrix4x4(const SPAtransf& acis_trans, QMatrix4x4& qt_matrix);

    void getEdgeFromReferenceLineSegment(EDGE* &edge, const ReferenceLineSegment& referenceLineSegment);

    void getReferenceLineWireBody(BODY *&wire_body, ENTITY_LIST& ents, std::vector<ReferenceLineSegment> &referenceLine, std::vector<EDGE*> &edges);

    void addPointToReferenceLine(std::vector<ReferenceLineSegment> &referenceLine, const QVector3D &new_point, const QString &curveType);

    void getParallelCurvePlanerBody(BODY* &new_body, BODY* &wire_body, ENTITY_LIST& ents, EDGE* &first_edge, float width, QString& referenceLinePosition);

    void addHelperPointsForLine(
        std::vector<ReferenceLineSegment> &referenceLine,
        ENTITY_LIST& ents,
        const QVector3D &point,
        const Point& screen_point,
        View *view,
        QList<HelperPoint> &helperPoints,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float
    );

    void addHelperPointsFor3PtCircle(
        std::vector<ReferenceLineSegment> &referenceLine,
        ENTITY_LIST& ents,
        const QVector3D &point,
        const Point& screen_point,
        View *view,
        QList<HelperPoint> &helperPoints,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float
    );

    void addHelperPointsForBezier(
        std::vector<ReferenceLineSegment> &referenceLine,
        ENTITY_LIST& ents,
        const QVector3D &point,
        const Point& screen_point,
        View *view,
        QList<HelperPoint> &helperPoints,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float
    );

    Point updatePointForLine(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST& ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view);

    Point updatePointFor3PtCircle(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST& ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view);

    Point updatePointForBezier(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST& ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view);


    /*
    void getMeshGeometry(const FacetModeler::Body& body, OdGePoint3dArray& pointArray, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, OdGeVector3dArray& normalArray);

    void getMeshGeometry(const OdMdBody& body, OdGePoint3dArray& pointArray, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, OdGeVector3dArray& normalArray);

    void getMeshGeometry(const BODY& body, OdGePoint3dArray& pointArray, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, OdGeVector3dArray& normalArray);

    void getMeshGeometry(const OdBrBrep& brep, OdGePoint3dArray& pointArray, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, OdGeVector3dArray& normalArray);
    */

    // std::vector<GLfloat> verticesVector
    void getMeshGeometry(
        const FacetModeler::Body& body,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float,
        int textureIndex,
        int materialIndex,
        int scalingFactor,
        float edgeWidth, // inputs to put in the std::vector<>
        float edgeDashLength,
        float edgeGapLength,
        int edgeDash,
        int edgeMaterialIndex // inputs to put in the std::vector<>
    );

    void getMeshGeometry(
        BODY* body,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float,
        int textureIndex,
        int materialIndex,
        int scalingFactor,
        float edgeWidth, // inputs to put in the std::vector<>
        float edgeDashLength,
        float edgeGapLength,
        int edgeDash,
        int edgeMaterialIndex // inputs to put in the std::vector<>
    );


    void getMeshGeometry(const OdMdBody& body,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float,
        int textureIndex,
        int materialIndex,
        int scalingFactor,
        float edgeWidth, // inputs to put in the std::vector<>
        float edgeDashLength,
        float edgeGapLength,
        int edgeDash,
        int edgeMaterialIndex // inputs to put in the std::vector<>
    );

    void getMeshGeometry(const BODY& body,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float,
        int textureIndex,
        int materialIndex,
        int scalingFactor,
        float edgeWidth, // inputs to put in the std::vector<>
        float edgeDashLength,
        float edgeGapLength,
        int edgeDash,
        int edgeMaterialIndex // inputs to put in the std::vector<>
    );

    void getMeshGeometry(const OdBrBrep& brep,
        std::vector<Position>& vertices_position,
        std::vector<Normal>& vertices_normal,
        std::vector<TextureUV>& vertices_textureuv,
        std::vector<int>& vertices_materialIndex,
        std::vector<int>& vertices_textureIndex,
        std::vector<uint32_t>& meshIndices,
        std::vector<int>& edge_indices,
        std::vector<EdgeDataInt>& edge_data_int,
        std::vector<EdgeDataFloat>& edge_data_float,
        int textureIndex,
        int materialIndex,
        int scalingFactor,
        float edgeWidth, // inputs to put in the std::vector<>
        float edgeDashLength,
        float edgeGapLength,
        int edgeDash,
        int edgeMaterialIndex // inputs to put in the std::vector<>
    );
};

#endif // OPENGL_HELPER_H
