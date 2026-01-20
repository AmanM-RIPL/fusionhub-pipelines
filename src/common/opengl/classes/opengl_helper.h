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
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"
#include "common/opengl/classes/opengl_material.h"
#include "common/opengl/classes/texture.h"


using Point = std::array<float, 2>; // (x,y)
using Line = std::array<float, 3>; // (m, b, x) for y = mx + b and x in case m is infinity



class OpenglHelper: public QObject
{
    Q_OBJECT
public:
    explicit OpenglHelper(QObject *parent = nullptr);

    void extractBIMParameters(BIMElement *wallElement, std::vector<Point>& referenceLine, float& width, float& height, float& distance);

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
        std::vector<EdgeIndex>& edge_indices,
        std::vector<float>& edge_width,
        std::vector<float>& edge_dashLength,
        std::vector<float>& edge_gapLength,
        std::vector<int>& edge_dash,
        std::vector<int>& edge_materialIndex,
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
        std::vector<EdgeIndex>& edge_indices,
        std::vector<float>& edge_width,
        std::vector<float>& edge_dashLength,
        std::vector<float>& edge_gapLength,
        std::vector<int>& edge_dash,
        std::vector<int>& edge_materialIndex,
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
        std::vector<EdgeIndex>& edge_indices,
        std::vector<float>& edge_width,
        std::vector<float>& edge_dashLength,
        std::vector<float>& edge_gapLength,
        std::vector<int>& edge_dash,
        std::vector<int>& edge_materialIndex,
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
        std::vector<EdgeIndex>& edge_indices,
        std::vector<float>& edge_width,
        std::vector<float>& edge_dashLength,
        std::vector<float>& edge_gapLength,
        std::vector<int>& edge_dash,
        std::vector<int>& edge_materialIndex,
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
