#include "opengl_helper.h"

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"

#include "IfcCore.h"
#include "IfcModel.h"
#include "IfcProduct.h"
//#include "FacetModeler/Body.h"
//#include "OdIfcRoot.h"


#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"

#include "common/myglitem.h"
#include "common/ifcdetail.h"

#include "database/database_manager.h"

#include "Br/BrBrep.h"
#include "BrepRenderer/BrepRendererImpl.h"
#include "Br/BrFace.h"
#include "Br/BrTraverser.h"

#include "Modeler/FMMdlBody.h"
#include "Modeler/FMDrawBody.h"
#include "Ge/GeCircArc2d.h"
#include <vector>

#include <QFile>
//#include <QDataStream>
#include <QTextStream>

// using namespace FacetModeler;
// using namespace std;

OpenglHelper::OpenglHelper(QObject *parent)
    : QObject{parent}
{}

void OpenglHelper::extractBIMParameters(
    BIMElement *wallElement,
    std::vector<ReferenceLineSegment>& referenceLine,
    std::vector<Layer>& layers,
    float& width,
    float& height,
    float& distance,
    float& slantAngle,
    float& taperAngle,
    QString& referenceLinePosition
)
{
    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    // 1. Find Reference Line parameter and convert to a list of list (2D)

    // 2. Find Width
    QString widthString = "0";
    QString heightString = "0";
    QString distanceString = "0";
    QString slantAngleString = "0";
    QString taperAngleString = "0";
    QString referenceLinePositionString = "inner";
    QString referenceLineString = "[]";
    QString layersString = "[]";

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            referenceLineString = parameter->getValue();
        }
        else if (parameter->getKey() == "Width")
        {
            widthString = parameter->getValue();
        }
        else if (parameter->getKey() == "Height")
        {
            heightString = parameter->getValue();
        }
        else if (parameter->getKey() == "Distance")
        {
            distanceString = parameter->getValue();
        }
        else if (parameter->getKey() == "Layers")
        {
            layersString = parameter->getValue();
        }
        else if (parameter->getKey() == "SlantAngle")
        {
            slantAngleString = parameter->getValue();
        }
        else if (parameter->getKey() == "TaperAngle")
        {
            taperAngleString = parameter->getValue();
        }
        else if (parameter->getKey() == "ReferenceLinePosition")
        {
            referenceLinePositionString = parameter->getValue();
        }
    }

    bool ok;
    width = widthString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }


    height = heightString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    distance = distanceString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    slantAngle = slantAngleString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    taperAngle = taperAngleString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    referenceLinePosition = referenceLinePositionString;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(referenceLineString.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& outerValue : jsonArray)
    {
        QJsonObject innerObject = outerValue.toObject();
        QString position = innerObject["position"].toString();
        QString type = innerObject["type"].toString();
        QJsonArray pointArray = innerObject["points"].toArray();

        std::vector<Point> pointList = {};
        for (const QJsonValue& pointValue : pointArray)
        {
            QJsonArray innerArray = pointValue.toArray();
            Point innerList = {innerArray.at(0).toDouble(), innerArray.at(1).toDouble()};
            pointList.push_back(innerList);
        }

        ReferenceLineSegment line_seg;
        line_seg.points = pointList;
        line_seg.type = type;

        referenceLine.push_back(line_seg);
    }

    // layer data
    QJsonDocument jsonDocLayers = QJsonDocument::fromJson(layersString.toUtf8());
    QJsonArray jsonArrayLayers = jsonDocLayers.array();

    for (const QJsonValue& outerValue : jsonArrayLayers)
    {
        QJsonObject innerObject = outerValue.toObject();
        QString name = innerObject["name"].toString();
        float width = static_cast<float>(innerObject["width"].toDouble());

        Layer layer;
        layer.name = name;
        layer.width = width;

        layers.push_back(layer);
    }

   // height = 4;
}

void OpenglHelper::extractBIMParameters(BIMElement *wallElement, std::vector<Point> &referenceLine, float &width, float &height, float& distance)
{
    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    // 1. Find Reference Line parameter and convert to a list of list (2D)

    // 2. Find Width
    QString widthString = "0";
    QString heightString = "0";
    QString distanceString = "0";
    QString referenceLineString = "[]";

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            // referenceLineString = parameter->getValue();
        }
        else if (parameter->getKey() == "Width")
        {
            widthString = parameter->getValue();
        }

        else if (parameter->getKey() == "Height")
        {
            heightString = parameter->getValue();
        }

        else if (parameter->getKey() == "Distance")
        {
            distanceString = parameter->getValue();
        }
    }

    bool ok;
    width = widthString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }


    height = heightString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    distance = distanceString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    // QJsonDocument jsonDoc = QJsonDocument::fromJson(referenceLineString.toUtf8());
    // QJsonArray jsonArray = jsonDoc.array();

    // for (const QJsonValue& outerValue : jsonArray)
    // {
    //     QJsonObject innerObject = outerValue.toObject();
    //     QString position = innerObject["position"].toString();
    //     QString type = innerObject["type"].toString();
    //     QJsonArray pointArray = innerObject["points"].toArray();

    //     std::vector<Point> pointList = {};
    //     for (const QJsonValue& pointValue : pointArray)
    //     {
    //         QJsonArray innerArray = pointValue.toArray();
    //         Point innerList = {innerArray.at(0).toDouble(), innerArray.at(1).toDouble()};
    //         pointList.push_back(innerList);
    //     }

    //     ReferenceLineSegment line_seg;
    //     line_seg.points = pointList;
    //     line_seg.position = position;
    //     line_seg.type = type;

    //     referenceLine.push_back(line_seg);
    // }

    // height = 4;
}


std::vector<Point> OpenglHelper::generateParallelCurve(std::vector<Point> referenceCurve, float width)
{
    std::vector<Point> parallelCurve;

    // Get Line Equation for first 2 points
    Point firstPoint = getParallelProjectionPoint(referenceCurve[0], referenceCurve[1], width);
    parallelCurve.insert(parallelCurve.begin(), firstPoint);

    // Get intersection points for the middle points
    if (referenceCurve.size() > 2)
    {
        for (int i = 1; i < referenceCurve.size() - 1; i++)
        {
            Line line1 = getParallelLineEquation(referenceCurve[i - 1], referenceCurve[i], width);
            Line line2 = getParallelLineEquation(referenceCurve[i], referenceCurve[i + 1], width);

            // in case both line have slope of infinity then we just parallely project it
            if (std::isinf(line1[0]) && std::isinf(line2[0]))
            {
                Point intersectionPoint = getParallelProjectionPoint(referenceCurve[i], referenceCurve[i + 1], width);
                parallelCurve.insert(parallelCurve.begin(), intersectionPoint);
            }
            else
            {
                Point intersectionPoint = getIntersectionPoint(line1, line2);
                parallelCurve.insert(parallelCurve.begin(), intersectionPoint);
            }
        }
    }

    // Get last point
    Point lastPoint = getParallelProjectionPoint(referenceCurve.back(), referenceCurve[referenceCurve.size() - 2], width);
    parallelCurve.insert(parallelCurve.begin(), lastPoint);

    return parallelCurve;
}

Line OpenglHelper::getParallelLineEquation(Point point1, Point point2, float width)
{
    // when slope is not infinite
    if (point2[0] - point1[0] != 0)
    {
        float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
        float b = point1[1] - point1[0] * m;
        float b_parallel = b + (width * qSqrt(qPow(m,2) + 1));

        Line result = {m, b_parallel, 0};

        return result;
    }

    Line result = {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), point1[0] - width};
    return result;
}

Point OpenglHelper::getIntersectionPoint(Line line1, Line line2)
{
    // both line1 and line2 cannot have m infinity
    // if they do then use the getParallelProjectionPoint logic
    if (std::isinf(line1[0]))
    {
        float x = line1[2];
        float y = line2[0] * x + line2[1];

        Point result = {x, y};
        return result;
    }
    else if (std::isinf(line2[0]))
    {
        float x = line2[2];
        float y = line1[0] * x + line1[1];

        Point result = {x, y};
        return result;
    }


    float x = (line2[1] - line1[1]) / (line1[0] - line2[0]);
    float y = line1[0] * x + line1[1];

    Point result = {x, y};

    return result;
}

Point OpenglHelper::getParallelProjectionPoint(Point point1, Point point2, float width)
{
    // m = (y2 - y1)/(x2-x1) if x2 = x1 then the slope is infinite.
    // so we handle that case seperately

    if (point2[0] - point1[0] != 0)
    {
        float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
        float x_proj = point1[0] + width * (-m/qSqrt(qPow(m,2) + 1));
        float y_proj = point1[1] + width * (1/qSqrt(qPow(m,2) + 1));

        Point result = {x_proj, y_proj};

        return result;
    }

    Point result = {point1[0] - width, point1[1]};
    return result;
}

Point OpenglHelper::getPointAtPerpendicularDistance(Point point1, Point point2, float distance)
{
    // vector from point 1 to 2 = (x2 - x1, y2 - y1)
    // vector length = sqrt((x2 - x1)^2 + (y2-y1)^2)
    // perpendicular vector = ((y2-y1)/length, -1 * (x2-x1)/length) -1 decides the point is on which side
    // point is = point1 + (distance * perpendicular vector)

    float x_proj = point2[0] - point1[0];
    float y_proj = point2[1] - point1[1];

    float length = qSqrt(qPow(x_proj, 2) + qPow(y_proj, 2));

    Point perpendicular_vector = {
        y_proj/length,
        -1 * x_proj/length
    };

    Point new_point = {
        point1[0] - (distance * perpendicular_vector[0]) ,
        point1[1] - (distance * perpendicular_vector[1])
    };

    return new_point;
}

Point OpenglHelper::getMiddlePoint(Point point1, Point point2)
{
    Point middle_point = {
        ((point2[0] - point1[0]) / 2) + point1[0],
        ((point2[1] - point1[1]) / 2) + point1[1]
    };

    return middle_point;
}

Point OpenglHelper::getPointAtDistance(Point point1, Point point2, float distance)
{
    float x_proj = point2[0] - point1[0];
    float y_proj = point2[1] - point1[1];

    float length = qSqrt(qPow(x_proj, 2) + qPow(y_proj, 2));
    float length_distance_ratio = distance / length;

    Point new_point = {
        ((point2[0] - point1[0]) * length_distance_ratio) + point1[0],
        ((point2[1] - point1[1]) * length_distance_ratio) + point1[1]
    };

    return new_point;
}

float OpenglHelper::getDistanceBetweenPoints(Point point1, Point point2)
{
    float x_proj = point2[0] - point1[0];
    float y_proj = point2[1] - point1[1];

    float length = qSqrt(qPow(x_proj, 2) + qPow(y_proj, 2));

    return length;
}

float OpenglHelper::getAngleBetweenPoints(Point point1, Point point2, Point point3)
{
    // first vector (from point2 towards point1)
    float x_proj1 = point1[0] - point2[0];
    float y_proj1 = point1[1] - point2[1];

    float length1 = qSqrt(qPow(x_proj1, 2) + qPow(y_proj1, 2));
    float x_proj1_unit = x_proj1/length1;
    float y_proj1_unit = y_proj1/length1;


    // second vector
    float x_proj2 = point3[0] - point2[0];
    float y_proj2 = point3[1] - point2[1];

    float length2 = qSqrt(qPow(x_proj2, 2) + qPow(y_proj2, 2));
    float x_proj2_unit = x_proj2/length2;
    float y_proj2_unit = y_proj2/length2;

    // dot product
    float dot_product = (x_proj1_unit * x_proj2_unit) + (y_proj1_unit * y_proj2_unit);
    float angle_radians = qAcos(dot_product);
    float angle_degrees = qRadiansToDegrees(angle_radians);

    return angle_degrees;
}

Point OpenglHelper::getPointAtDistanceAngle(Point point1, Point point2, float angle, float distance)
{
    // angle in radians
    float angle_radians = qDegreesToRadians(angle);

    // first vector (from point2 towards point1)
    float x_proj1 = point1[0] - point2[0];
    float y_proj1 = point1[1] - point2[1];

    float length1 = qSqrt(qPow(x_proj1, 2) + qPow(y_proj1, 2));
    float x_proj1_unit = x_proj1/length1;
    float y_proj1_unit = y_proj1/length1;

    // rotated unit vector
    float x_proj_rotated_unit = (x_proj1_unit * qCos(angle_radians)) - (y_proj1_unit * qSin(angle_radians));
    float y_proj_rotated_unit = (x_proj1_unit * qSin(angle_radians)) + (y_proj1_unit * qCos(angle_radians));

    return {
        x_proj_rotated_unit * distance + point2[0],
        y_proj_rotated_unit * distance + point2[1]
    };
}

void OpenglHelper::convertSPAtransfToQMatrix4x4(const SPAtransf &acis_trans, QMatrix4x4 &qt_matrix)
{
    SPAmatrix acis_matrix = acis_trans.affine();
    SPAvector acis_vector = acis_trans.translation();
    qt_matrix = QMatrix4x4(
        acis_matrix.element(0,0), acis_matrix.element(0,1), acis_matrix.element(0,2), acis_vector.x(),
        acis_matrix.element(1,0), acis_matrix.element(1,1), acis_matrix.element(1,2), acis_vector.y(),
        acis_matrix.element(2,0), acis_matrix.element(2,1), acis_matrix.element(2,2), acis_vector.z(),
        0.0, 0.0, 0.0, 1.0
    );
}

void OpenglHelper::getEdgeFromReferenceLineSegment(EDGE* &edge, const ReferenceLineSegment &referenceLineSegment)
{
    // we allow circle with 2 points to be shown as a line and 3-point bezier to be shown as a circle
    // for easy viewing for the user. Can be changed if required

    if ((referenceLineSegment.type == "line" || referenceLineSegment.type == "3pt-circle") && referenceLineSegment.points.size() == 2)
    {
        api_curve_line(
            SPAposition(referenceLineSegment.points[0][0], referenceLineSegment.points[0][1], 0.0),
            SPAposition(referenceLineSegment.points[1][0], referenceLineSegment.points[1][1], 0.0),
            edge
        );
    }
    else if ((referenceLineSegment.type == "3pt-circle") && referenceLineSegment.points.size() == 3)
    {
        api_curve_arc_3pt(
            SPAposition(referenceLineSegment.points[0][0], referenceLineSegment.points[0][1], 0.0),
            SPAposition(referenceLineSegment.points[1][0], referenceLineSegment.points[1][1], 0.0),
            SPAposition(referenceLineSegment.points[2][0], referenceLineSegment.points[2][1], 0.0),
            false,
            edge
        );
    }
    else if (referenceLineSegment.type == "bezier" && referenceLineSegment.points.size() == 4)
    {
        api_curve_bezier(
            SPAposition(referenceLineSegment.points[0][0], referenceLineSegment.points[0][1], 0.0),
            SPAposition(referenceLineSegment.points[1][0], referenceLineSegment.points[1][1], 0.0),
            SPAposition(referenceLineSegment.points[2][0], referenceLineSegment.points[2][1], 0.0),
            SPAposition(referenceLineSegment.points[3][0], referenceLineSegment.points[3][1], 0.0),
            edge
        );
    }
}

void OpenglHelper::getReferenceLineWireBody(BODY *&wire_body, ENTITY_LIST& ents, std::vector<ReferenceLineSegment> &referenceLine, std::vector<EDGE*> &edges)
{
    for (ReferenceLineSegment& line_seg: referenceLine)
    {
        EDGE* edge = nullptr;
        ents.add(edge);

        getEdgeFromReferenceLineSegment(edge, line_seg);

        if (edge == nullptr) continue;

        edges.push_back(edge);
    }

    // don't proceed further if there are no edges
    if (edges.size() == 0) return;


    api_make_ewire(edges.size(), edges.data(), wire_body);
}

void OpenglHelper::addPointToReferenceLine(std::vector<ReferenceLineSegment> &referenceLine, const QVector3D &new_point, const QString &curveType)
{
    // 1. Get the last ReferenceLineSegment
    if (referenceLine.size() == 0)
    {
        ReferenceLineSegment rls;
        rls.points = { { new_point.x(), new_point.y() } };
        rls.type = curveType;

        referenceLine.push_back(rls);

        return;
    }

    ReferenceLineSegment& last_rls = referenceLine.back();

    // 2. Check if all points are there in rls based on curve type
    bool canAddAnotherPointToRLS = false;

    if (last_rls.type == "line" && last_rls.points.size() < 2)
    {
        canAddAnotherPointToRLS = true;
    }
    else if (last_rls.type == "3pt-circle" && last_rls.points.size() < 3)
    {
        canAddAnotherPointToRLS = true;
    }
    else if (last_rls.type == "bezier" && last_rls.points.size() < 4)
    {
        canAddAnotherPointToRLS = true;
    }

    // 3. Update point based on above input
    if (canAddAnotherPointToRLS && (last_rls.type == curveType))
    {
        last_rls.points.push_back({ new_point.x(), new_point.y() });
    }
    else if (canAddAnotherPointToRLS && (last_rls.type != curveType))
    {
        // last_rls always have one point in .points as per point 1.
        last_rls.points = {
            last_rls.points[0],
            { new_point.x(), new_point.y() }
        };

        last_rls.type = curveType;
    }
    else if (!canAddAnotherPointToRLS)
    {
        ReferenceLineSegment rls;
        rls.points = {
            last_rls.points.back(),
            { new_point.x(), new_point.y() }
        };
        rls.type = curveType;

        referenceLine.push_back(rls);
    }
}

void OpenglHelper::getParallelCurvePlanerBody(BODY *&new_body, BODY* &wire_body, ENTITY_LIST &ents, EDGE *&first_edge, float width, QString& referenceLinePosition)
{

    // create edge perpendicular to the first edge of the wire_body
    SPAposition first_edge_point = first_edge->start_pos();

    SPAvector tangent_vector = first_edge->start_deriv();

    // use referenceLinePosition to get the perpendicular vector
    float perp_x = referenceLinePosition == "outer" ? tangent_vector.y() : -1 * tangent_vector.y();
    float perp_y = referenceLinePosition == "outer" ? -1 * tangent_vector.x() : tangent_vector.x();
    SPAvector perp_vector(perp_x, perp_y, tangent_vector.z());
    perp_vector = (width / perp_vector.len()) * perp_vector;

    SPAposition profile_point(
        perp_vector.x() + first_edge_point.x(),
        perp_vector.y() + first_edge_point.y(),
        perp_vector.z()
    );

    EDGE* edge_for_sweep = nullptr;
    ents.add(edge_for_sweep);
    api_curve_line(first_edge_point, profile_point, edge_for_sweep);

    // do sweep
    EXCEPTION_BEGIN
        sweep_options* sw_options = ACIS_NEW sweep_options();
    EXCEPTION_TRY
        outcome sw_result = api_sweep_with_options(edge_for_sweep, wire_body, sw_options, new_body);

        if (!sw_result.ok())
        {
            error_info* info = sw_result.get_error_info();
            qInfo() << info->error_message();
        }

    EXCEPTION_CATCH_TRUE
        ACIS_DELETE sw_options;
    EXCEPTION_END
}

void OpenglHelper::addHelperPointsForLine(
    std::vector<ReferenceLineSegment> &referenceLine,
    ENTITY_LIST& ents,
    const QVector3D &point,
    const Point& screen_point,
    View *view, QList<HelperPoint> &helperPoints,
    std::vector<Position>& vertices_position,
    std::vector<Normal>& vertices_normal,
    std::vector<TextureUV>& vertices_textureuv,
    std::vector<int>& vertices_materialIndex,
    std::vector<int>& vertices_textureIndex,
    std::vector<uint32_t>& meshIndices,
    std::vector<int>& edge_indices,
    std::vector<EdgeDataInt>& edge_data_int,
    std::vector<EdgeDataFloat>& edge_data_float
)
{
    const ReferenceLineSegment& last_rls = referenceLine.back();

    // 1. ignore if type is not a line
    if (last_rls.type != "line") return;

    // 2. need at least two points to show the line helper points
    if (last_rls.points.size() < 2) return;

    // 3. update the length parameter at helperPoints[0]

    // 3.1 show length helper point
    helperPoints[0].visible = true;

    // 3.2 calculate helperPoints in WCS
    Point lastPointReferenceLine = last_rls.points[0];
    QVector3D lastPoint(lastPointReferenceLine[0], lastPointReferenceLine[1], 0.0f);
    Point lastPointScreenSpace = view->GetPointInScreenSpace(lastPoint);
    Point middlePointScreenSpace = getMiddlePoint(lastPointScreenSpace, screen_point);

    Point new_point1 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 4);
    Point new_point2 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 10);
    Point new_point3 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -10);
    Point new_point4 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -4);
    Point middle_point = getPointAtPerpendicularDistance(middlePointScreenSpace, screen_point, 10);

    helperPoints[0].x = middle_point[0];
    helperPoints[0].y = middle_point[1] + 30; // 30px added to offset

    QVector3D wcs_point1 = view->GetPointInViewSpace(new_point1[0], new_point1[1]);
    QVector3D wcs_point2 = view->GetPointInViewSpace(new_point2[0], new_point2[1]);
    QVector3D wcs_point3 = view->GetPointInViewSpace(new_point3[0], new_point3[1]);
    QVector3D wcs_point4 = view->GetPointInViewSpace(new_point4[0], new_point4[1]);

    helperPoints[0].value = getDistanceBetweenPoints(lastPointReferenceLine, {point[0], point[1]});

    // 3.3 Add the vertex and edges of helper lines
    vertices_position.push_back({wcs_point1[0], wcs_point1[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point2[0], wcs_point2[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point3[0], wcs_point3[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point4[0], wcs_point4[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    for (int i = 0; i < 3; i++)
    {
        // - 4 because we have added four points above
        int first_point = (vertices_position.size() - 4) + i;
        int second_point = (vertices_position.size() - 4) + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 1;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }

    // 4. Calculate angle helper point if referenceLine has more than one rls
    if (referenceLine.size() > 1)
    {
        // show the angle helper point
        helperPoints[1].visible = true;

        const ReferenceLineSegment& second_last_rls = referenceLine[referenceLine.size() - 2];

        // 4.1 convert to edge and get the reversed tangent vector
        EDGE* edge = nullptr;
        ents.add(edge);

        getEdgeFromReferenceLineSegment(edge, second_last_rls);

        if (edge == nullptr) return;

        SPAposition last_edge_point = edge->end_pos();
        SPAvector tangent_vector = edge->end_deriv();
        SPAvector opposite_vector = -1 * tangent_vector;

        Point secondLastPointReferenceLine = {
            last_edge_point.x() + opposite_vector.x(),
            last_edge_point.y() + opposite_vector.y()
        };
        QVector3D secondLastPoint(secondLastPointReferenceLine[0], secondLastPointReferenceLine[1], 0.0f);
        Point secondLastPointScreenSpace = view->GetPointInScreenSpace(secondLastPoint);

        float angle_degrees = getAngleBetweenPoints(secondLastPointReferenceLine, lastPointReferenceLine, {point[0], point[1]});

        // angles are negative because the screen-coordinates are following the left-handle rule
        // y axis points downwards
        Point angle_point0 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, 0.0f, 20);
        Point angle_point1 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/4, 20);
        Point angle_point2 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/2, 20);
        Point angle_point3 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -3*angle_degrees/4, 20);
        Point angle_point4 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees, 20);

        QVector3D wcs_angle_point0 = view->GetPointInViewSpace(angle_point0[0], angle_point0[1]);
        QVector3D wcs_angle_point1 = view->GetPointInViewSpace(angle_point1[0], angle_point1[1]);
        QVector3D wcs_angle_point2 = view->GetPointInViewSpace(angle_point2[0], angle_point2[1]);
        QVector3D wcs_angle_point3 = view->GetPointInViewSpace(angle_point3[0], angle_point3[1]);
        QVector3D wcs_angle_point4 = view->GetPointInViewSpace(angle_point4[0], angle_point4[1]);


        helperPoints[1].x = angle_point2[0];
        helperPoints[1].y = angle_point2[1] + 30; // go 30 px down
        helperPoints[1].value = angle_degrees;

        // Add the vertex and edges of helper lines
        vertices_position.push_back({wcs_angle_point0[0], wcs_angle_point0[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point1[0], wcs_angle_point1[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point2[0], wcs_angle_point2[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point3[0], wcs_angle_point3[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point4[0], wcs_angle_point4[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);


        for (int i = 0; i < 4; i++)
        {
            // 4 is added to account for length helper point
            int first_point = (vertices_position.size() - 5) + i;
            int second_point = (vertices_position.size() - 5) + i + 1;

            EdgeDataInt ei;
            EdgeDataFloat ef;

            ei.material_index = OpenGLMaterial::BLACK;
            ei.dash = 1;
            ef.width = 2.0f;
            ef.dash_length = 5.0f;
            ef.gap_length = 5.0f;
            ef.padding = 0.0f;

            ei.start_vertex = first_point;
            ei.end_vertex = second_point;

            edge_data_int.push_back(ei);
            edge_data_float.push_back(ef);
            edge_indices.push_back(edge_indices.size());
        }
    }
}

void OpenglHelper::addHelperPointsFor3PtCircle(
    std::vector<ReferenceLineSegment> &referenceLine,
    ENTITY_LIST &ents,
    const QVector3D &point,
    const Point &screen_point,
    View *view,
    QList<HelperPoint> &helperPoints,
    std::vector<Position> &vertices_position,
    std::vector<Normal> &vertices_normal,
    std::vector<TextureUV> &vertices_textureuv,
    std::vector<int> &vertices_materialIndex,
    std::vector<int> &vertices_textureIndex,
    std::vector<uint32_t> &meshIndices,
    std::vector<int> &edge_indices,
    std::vector<EdgeDataInt> &edge_data_int,
    std::vector<EdgeDataFloat> &edge_data_float
)
{
    const ReferenceLineSegment& last_rls = referenceLine.back();

    // 1. ignore if type is not a 3pt-cicle
    if (last_rls.type != "3pt-circle") return;

    // 2. need at least two points to show the 3pt-circle helper points
    if (last_rls.points.size() < 2) return;

    // 3. update the length parameter at helperPoints[0]

    // 3.1 show length helper point
    helperPoints[0].visible = true;

    // 3.2 calculate helperPoints in WCS
    Point lastPointReferenceLine = last_rls.points[last_rls.points.size() - 2]; // get the second last point
    QVector3D lastPoint(lastPointReferenceLine[0], lastPointReferenceLine[1], 0.0f);
    Point lastPointScreenSpace = view->GetPointInScreenSpace(lastPoint);
    Point middlePointScreenSpace = getMiddlePoint(lastPointScreenSpace, screen_point);

    Point new_point1 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 4);
    Point new_point2 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 10);
    Point new_point3 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -10);
    Point new_point4 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -4);
    Point middle_point = getPointAtPerpendicularDistance(middlePointScreenSpace, screen_point, 10);

    helperPoints[0].x = middle_point[0];
    helperPoints[0].y = middle_point[1] + 30; // 30px added to offset

    QVector3D wcs_point1 = view->GetPointInViewSpace(new_point1[0], new_point1[1]);
    QVector3D wcs_point2 = view->GetPointInViewSpace(new_point2[0], new_point2[1]);
    QVector3D wcs_point3 = view->GetPointInViewSpace(new_point3[0], new_point3[1]);
    QVector3D wcs_point4 = view->GetPointInViewSpace(new_point4[0], new_point4[1]);

    helperPoints[0].value = getDistanceBetweenPoints(lastPointReferenceLine, {point[0], point[1]});

    // 3.3 Add the vertex and edges of helper lines
    vertices_position.push_back({wcs_point1[0], wcs_point1[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point2[0], wcs_point2[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point3[0], wcs_point3[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point4[0], wcs_point4[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    for (int i = 0; i < 3; i++)
    {
        // - 4 because we have added four points above
        int first_point = (vertices_position.size() - 4) + i;
        int second_point = (vertices_position.size() - 4) + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 1;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }

    // 4. Get the angle relative to x-axis
    // show the angle helper point
    helperPoints[1].visible = true;

    Point secondLastPointReferenceLine = {
        lastPointReferenceLine[0] + 3.0f,
        lastPointReferenceLine[1]
    };
    QVector3D secondLastPoint(secondLastPointReferenceLine[0], secondLastPointReferenceLine[1], 0.0f);
    Point secondLastPointScreenSpace = view->GetPointInScreenSpace(secondLastPoint);

    float angle_degrees = getAngleBetweenPoints(secondLastPointReferenceLine, lastPointReferenceLine, {point[0], point[1]});

    // angles are negative because the screen-coordinates are following the left-handle rule
    // y axis points downwards
    Point angle_point0 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, 0.0f, 20);
    Point angle_point1 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/4, 20);
    Point angle_point2 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/2, 20);
    Point angle_point3 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -3*angle_degrees/4, 20);
    Point angle_point4 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees, 20);

    QVector3D wcs_angle_point0 = view->GetPointInViewSpace(angle_point0[0], angle_point0[1]);
    QVector3D wcs_angle_point1 = view->GetPointInViewSpace(angle_point1[0], angle_point1[1]);
    QVector3D wcs_angle_point2 = view->GetPointInViewSpace(angle_point2[0], angle_point2[1]);
    QVector3D wcs_angle_point3 = view->GetPointInViewSpace(angle_point3[0], angle_point3[1]);
    QVector3D wcs_angle_point4 = view->GetPointInViewSpace(angle_point4[0], angle_point4[1]);


    helperPoints[1].x = angle_point2[0];
    helperPoints[1].y = angle_point2[1] + 30; // go 30 px down
    helperPoints[1].value = angle_degrees;

    // Add the vertex and edges of helper lines
    vertices_position.push_back({wcs_angle_point0[0], wcs_angle_point0[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_angle_point1[0], wcs_angle_point1[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_angle_point2[0], wcs_angle_point2[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_angle_point3[0], wcs_angle_point3[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_angle_point4[0], wcs_angle_point4[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({lastPointReferenceLine[0], lastPointReferenceLine[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({secondLastPointReferenceLine[0], secondLastPointReferenceLine[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    // edges for angle
    for (int i = 0; i < 4; i++)
    {
        // 7 is added to account for length helper point
        int first_point = (vertices_position.size() - 7) + i;
        int second_point = (vertices_position.size() - 7) + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 1;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }

    // edges for x-axis
    int first_point = vertices_position.size() - 2;
    int second_point = vertices_position.size() - 1;

    EdgeDataInt ei;
    EdgeDataFloat ef;

    ei.material_index = OpenGLMaterial::BLACK;
    ei.dash = 1;
    ef.width = 2.0f;
    ef.dash_length = 5.0f;
    ef.gap_length = 5.0f;
    ef.padding = 0.0f;

    ei.start_vertex = first_point;
    ei.end_vertex = second_point;

    edge_data_int.push_back(ei);
    edge_data_float.push_back(ef);
    edge_indices.push_back(edge_indices.size());
}

void OpenglHelper::addHelperPointsForBezier(
    std::vector<ReferenceLineSegment> &referenceLine,
    ENTITY_LIST &ents,
    const QVector3D &point,
    const Point &screen_point,
    View *view,
    QList<HelperPoint> &helperPoints,
    std::vector<Position> &vertices_position,
    std::vector<Normal> &vertices_normal,
    std::vector<TextureUV> &vertices_textureuv,
    std::vector<int> &vertices_materialIndex,
    std::vector<int> &vertices_textureIndex,
    std::vector<uint32_t> &meshIndices,
    std::vector<int> &edge_indices,
    std::vector<EdgeDataInt> &edge_data_int,
    std::vector<EdgeDataFloat> &edge_data_float
)
{
    const ReferenceLineSegment& last_rls = referenceLine.back();

    // 1. ignore if type is not a bezier
    if (last_rls.type != "bezier") return;

    // 2. need at least two points to show the bezier helper points
    if (last_rls.points.size() < 2) return;

    // 2.1 draw edges of the control points
    for (int i = 0; i < last_rls.points.size(); i++)
    {
        // add the points
        vertices_position.push_back({last_rls.points[i][0], last_rls.points[i][1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);
    }

    for (int i = 0; i < last_rls.points.size() - 1; i++)
    {
        // add the edges
        int first_point = (vertices_position.size() - last_rls.points.size()) + i;
        int second_point = (vertices_position.size() - last_rls.points.size()) + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 0;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }

    // 3. update the length parameter at helperPoints[0]

    // 3.1 show length helper point
    helperPoints[0].visible = true;

    // 3.2 calculate helperPoints in WCS
    Point lastPointReferenceLine = last_rls.points[last_rls.points.size() - 2]; // get the second last point
    QVector3D lastPoint(lastPointReferenceLine[0], lastPointReferenceLine[1], 0.0f);
    Point lastPointScreenSpace = view->GetPointInScreenSpace(lastPoint);
    Point middlePointScreenSpace = getMiddlePoint(lastPointScreenSpace, screen_point);

    Point new_point1 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 4);
    Point new_point2 = getPointAtPerpendicularDistance(lastPointScreenSpace, screen_point, 10);
    Point new_point3 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -10);
    Point new_point4 = getPointAtPerpendicularDistance(screen_point, lastPointScreenSpace, -4);
    Point middle_point = getPointAtPerpendicularDistance(middlePointScreenSpace, screen_point, 10);

    helperPoints[0].x = middle_point[0];
    helperPoints[0].y = middle_point[1] + 30; // 30px added to offset

    QVector3D wcs_point1 = view->GetPointInViewSpace(new_point1[0], new_point1[1]);
    QVector3D wcs_point2 = view->GetPointInViewSpace(new_point2[0], new_point2[1]);
    QVector3D wcs_point3 = view->GetPointInViewSpace(new_point3[0], new_point3[1]);
    QVector3D wcs_point4 = view->GetPointInViewSpace(new_point4[0], new_point4[1]);

    helperPoints[0].value = getDistanceBetweenPoints(lastPointReferenceLine, {point[0], point[1]});

    // 3.3 Add the vertex and edges of helper lines
    vertices_position.push_back({wcs_point1[0], wcs_point1[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point2[0], wcs_point2[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point3[0], wcs_point3[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    vertices_position.push_back({wcs_point4[0], wcs_point4[1], 0.0f, 0.0f});
    vertices_normal.push_back({0.0f, 0.0f, 1.0f});
    vertices_textureuv.push_back({0.0f, 0.0f});
    vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
    vertices_textureIndex.push_back(Texture::NONE);

    for (int i = 0; i < 3; i++)
    {
        // - 4 because we have added four points above
        int first_point = (vertices_position.size() - 4) + i;
        int second_point = (vertices_position.size() - 4) + i + 1;

        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 1;
        ef.width = 2.0f;
        ef.dash_length = 5.0f;
        ef.gap_length = 5.0f;
        ef.padding = 0.0f;

        ei.start_vertex = first_point;
        ei.end_vertex = second_point;

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(edge_indices.size());
    }

    // 4. Get the angle relative to third last bezier point
    if (last_rls.points.size() > 2)
    {
        // show the angle helper point
        helperPoints[1].visible = true;

        Point secondLastPointReferenceLine = last_rls.points[last_rls.points.size() - 3];
        QVector3D secondLastPoint(secondLastPointReferenceLine[0], secondLastPointReferenceLine[1], 0.0f);
        Point secondLastPointScreenSpace = view->GetPointInScreenSpace(secondLastPoint);

        float angle_degrees = getAngleBetweenPoints(secondLastPointReferenceLine, lastPointReferenceLine, {point[0], point[1]});

        // angles are negative because the screen-coordinates are following the left-handle rule
        // y axis points downwards
        Point angle_point0 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, 0.0f, 20);
        Point angle_point1 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/4, 20);
        Point angle_point2 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees/2, 20);
        Point angle_point3 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -3*angle_degrees/4, 20);
        Point angle_point4 = getPointAtDistanceAngle(secondLastPointScreenSpace, lastPointScreenSpace, -1*angle_degrees, 20);

        QVector3D wcs_angle_point0 = view->GetPointInViewSpace(angle_point0[0], angle_point0[1]);
        QVector3D wcs_angle_point1 = view->GetPointInViewSpace(angle_point1[0], angle_point1[1]);
        QVector3D wcs_angle_point2 = view->GetPointInViewSpace(angle_point2[0], angle_point2[1]);
        QVector3D wcs_angle_point3 = view->GetPointInViewSpace(angle_point3[0], angle_point3[1]);
        QVector3D wcs_angle_point4 = view->GetPointInViewSpace(angle_point4[0], angle_point4[1]);


        helperPoints[1].x = angle_point2[0];
        helperPoints[1].y = angle_point2[1] + 30; // go 30 px down
        helperPoints[1].value = angle_degrees;

        // Add the vertex and edges of helper lines
        vertices_position.push_back({wcs_angle_point0[0], wcs_angle_point0[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point1[0], wcs_angle_point1[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point2[0], wcs_angle_point2[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point3[0], wcs_angle_point3[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        vertices_position.push_back({wcs_angle_point4[0], wcs_angle_point4[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        // edges for angle
        for (int i = 0; i < 4; i++)
        {
            // 7 is added to account for length helper point
            int first_point = (vertices_position.size() - 5) + i;
            int second_point = (vertices_position.size() - 5) + i + 1;

            EdgeDataInt ei;
            EdgeDataFloat ef;

            ei.material_index = OpenGLMaterial::BLACK;
            ei.dash = 1;
            ef.width = 2.0f;
            ef.dash_length = 5.0f;
            ef.gap_length = 5.0f;
            ef.padding = 0.0f;

            ei.start_vertex = first_point;
            ei.end_vertex = second_point;

            edge_data_int.push_back(ei);
            edge_data_float.push_back(ef);
            edge_indices.push_back(edge_indices.size());
        }
    }
}

Point OpenglHelper::updatePointForLine(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST &ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view)
{
    // 1. Get last point of referenceLine
    ReferenceLineSegment& last_rls = referenceLine.back();
    Point lastPointReferenceLine = last_rls.points.back();

    QVector3D screenPointInViewSpace = view->GetPointInViewSpace(screen_point[0], screen_point[1]);
    Point screenPointInViewSpace2D = {screenPointInViewSpace[0], screenPointInViewSpace[1]};

    Point newPointViewSpace = {0.0f, 0.0f};

    if (referenceLine.size() < 2)
    {
        newPointViewSpace = getPointAtDistance(lastPointReferenceLine, screenPointInViewSpace2D, helperPoints[0].value);
    }
    else
    {
        ReferenceLineSegment& second_last_rls = referenceLine[referenceLine.size() - 2];
        EDGE* edge = nullptr;
        ents.add(edge);

        getEdgeFromReferenceLineSegment(edge, second_last_rls);

        SPAposition last_edge_point = edge->end_pos();
        SPAvector tangent_vector = edge->end_deriv();
        SPAvector opposite_vector = -1 * tangent_vector;

        Point secondLastPointReferenceLine = {
            last_edge_point.x() + opposite_vector.x(),
            last_edge_point.y() + opposite_vector.y()
        };

        newPointViewSpace = getPointAtDistanceAngle(secondLastPointReferenceLine, lastPointReferenceLine, helperPoints[1].value, helperPoints[0].value);
    }

    QVector3D newPoint(newPointViewSpace[0], newPointViewSpace[1], 0.0f);

    Point newPointScreenSpace = view->GetPointInScreenSpace(newPoint);

    return newPointScreenSpace;
}

Point OpenglHelper::updatePointFor3PtCircle(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST &ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view)
{
    // 1. Get last point of referenceLine
    ReferenceLineSegment& last_rls = referenceLine.back();
    Point lastPointReferenceLine = last_rls.points.back();

    QVector3D screenPointInViewSpace = view->GetPointInViewSpace(screen_point[0], screen_point[1]);
    Point screenPointInViewSpace2D = {screenPointInViewSpace[0], screenPointInViewSpace[1]};

    Point newPointViewSpace = {0.0f, 0.0f};

    if (last_rls.points.size() < 2)
    {
        newPointViewSpace = getPointAtDistance(lastPointReferenceLine, screenPointInViewSpace2D, helperPoints[0].value);
    }
    else
    {
        Point secondLastPointReferenceLine = {
            lastPointReferenceLine[0] + 3.0f,
            lastPointReferenceLine[1]
        };

        newPointViewSpace = getPointAtDistanceAngle(secondLastPointReferenceLine, lastPointReferenceLine, helperPoints[1].value, helperPoints[0].value);
    }

    QVector3D newPoint(newPointViewSpace[0], newPointViewSpace[1], 0.0f);

    Point newPointScreenSpace = view->GetPointInScreenSpace(newPoint);

    return newPointScreenSpace;
}

Point OpenglHelper::updatePointForBezier(std::vector<ReferenceLineSegment> &referenceLine, ENTITY_LIST &ents, const QList<HelperPoint> &helperPoints, const Point &screen_point, View *view)
{
    // 1. Get last point of referenceLine
    ReferenceLineSegment& last_rls = referenceLine.back();
    Point lastPointReferenceLine = last_rls.points.back();

    QVector3D screenPointInViewSpace = view->GetPointInViewSpace(screen_point[0], screen_point[1]);
    Point screenPointInViewSpace2D = {screenPointInViewSpace[0], screenPointInViewSpace[1]};

    Point newPointViewSpace = {0.0f, 0.0f};

    if (last_rls.points.size() < 2)
    {
        newPointViewSpace = getPointAtDistance(lastPointReferenceLine, screenPointInViewSpace2D, helperPoints[0].value);
    }
    else
    {
        Point secondLastPointReferenceLine = last_rls.points[last_rls.points.size() - 2];

        newPointViewSpace = getPointAtDistanceAngle(secondLastPointReferenceLine, lastPointReferenceLine, helperPoints[1].value, helperPoints[0].value);
    }

    QVector3D newPoint(newPointViewSpace[0], newPointViewSpace[1], 0.0f);

    Point newPointScreenSpace = view->GetPointInScreenSpace(newPoint);

    return newPointScreenSpace;
}


void OpenglHelper::getMeshGeometry(
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
)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    if (textureIndex < 0)
    {
        FacetModeler::Face* face = body.faceList();
        for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
        {
            OdGePoint3dArray facePointArray = {};
            std::vector<int> numOfEdgesInLoop = {};
            std::vector<std::vector<Point>> earcutPolygon = {};

            // Face Plane Coordinate System
            OdGePoint3d origin;
            OdGeVector3d axis1;
            OdGeVector3d axis2; // this is in the plane not normal to it.
            OdGeVector3d normal = face->normal();

            face->plane().getCoordSystem(origin, axis1, axis2);

            for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
            {
                std::vector<Point> loopPolygon = {};
                numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

                FacetModeler::Edge* edge = face->edge(loopIndex);
                for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
                {
                    facePointArray.append(edge->startPoint());

                    unsigned int startPointIndex = 0;
                    bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                    if (!startResult)
                    {
                        pointArray.append(edge->startPoint());
                        normalArray.append(normal);
                        textureArray.push_back({0.0f, 0.0f});

                        startPointIndex = pointArray.size() - 1;
                    }


                    unsigned int endPointIndex = 0;
                    bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                    if (!endResult)
                    {
                        pointArray.append(edge->endPoint());
                        normalArray.append(normal);
                        textureArray.push_back({0.0f, 0.0f});

                        endPointIndex = pointArray.size() - 1;
                    }

                    edge_indices.push_back(edge_indices.size());
                    EdgeDataInt ei;
                    EdgeDataFloat ef;

                    ei.material_index = edgeMaterialIndex;
                    ei.dash = edgeDash;
                    ef.width = edgeWidth;
                    ef.dash_length = edgeDashLength;
                    ef.gap_length = edgeGapLength;
                    ef.padding = 0.0f;

                    ei.start_vertex = static_cast<int>(startPointIndex);
                    ei.end_vertex = static_cast<int>(endPointIndex);

                    edge_data_int.push_back(ei);
                    edge_data_float.push_back(ef);

                    // get two dimensional point on the Face Plane
                    OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                    float x = vecOnPlane.dotProduct(axis1);
                    float y = vecOnPlane.dotProduct(axis2);
                    loopPolygon.push_back({x,y});

                    edge = edge->next();
                }

                earcutPolygon.push_back(loopPolygon);
            }

            // generate the 2D tessellations
            std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

            // project index from indices to pointArray
            for (uint32_t index: indices)
            {
                OdGePoint3d pointInFacePointArray = facePointArray[index];

                unsigned int selectedIndex = 0;
                bool result = pointArray.find(pointInFacePointArray, selectedIndex);

                meshIndices.push_back(selectedIndex);
                normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
                normalArray[selectedIndex].normalize();
            }

            face = face->next();
        }
    }
    else
    {
        FacetModeler::Face* face = body.faceList();
        for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
        {
            OdGePoint3dArray facePointArray = {};
            std::vector<std::array<float, 2>> faceTextureArray = {};
            std::vector<std::array<int, 2>> faceBorderIndices = {};
            std::vector<std::vector<Point>> earcutPolygon = {};

            // Face Plane Coordinate System
            OdGePoint3d origin;
            OdGeVector3d axis1;
            OdGeVector3d axis2; // this is in the plane not normal to it.
            OdGeVector3d normal = face->normal();

            face->plane().getCoordSystem(origin, axis1, axis2);

            for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
            {
                std::vector<Point> loopPolygon = {};

                FacetModeler::Edge* edge = face->edge(loopIndex);
                unsigned int startingFacePointIndex = facePointArray.size();
                for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
                {
                    facePointArray.append(edge->startPoint());

                    if (edgeIndex < face->loopEdgeCount(loopIndex) - 1)
                    {
                        faceBorderIndices.push_back({
                            static_cast<int>(facePointArray.size() - 1),
                            static_cast<int>(facePointArray.size())
                        }); // for second point
                    }
                    else
                    {
                        faceBorderIndices.push_back({
                            static_cast<int>(facePointArray.size() - 1),
                            static_cast<int>(startingFacePointIndex)
                        }); // for last point the end point is first point
                    }

                    // get two dimensional point on the Face Plane
                    OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                    float x = vecOnPlane.dotProduct(axis1);
                    float y = vecOnPlane.dotProduct(axis2);
                    loopPolygon.push_back({x,y});
                    faceTextureArray.push_back({x/scalingFactor, y/scalingFactor});

                    edge = edge->next();
                }

                earcutPolygon.push_back(loopPolygon);
            }

            // generate the 2D tessellations
            std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

            // project index from indices to pointArray
            int startingPointArrayIndex = pointArray.size();
            for (uint32_t index: indices)
            {
                meshIndices.push_back(startingPointArrayIndex + index);
            }

            for (std::array<int, 2> faceBorderIndex: faceBorderIndices)
            {   
                edge_indices.push_back(edge_indices.size());
                EdgeDataInt ei;
                EdgeDataFloat ef;

                ei.material_index = edgeMaterialIndex;
                ei.dash = edgeDash;
                ef.width = edgeWidth;
                ef.dash_length = edgeDashLength;
                ef.gap_length = edgeGapLength;
                ef.padding = 0.0f;

                ei.start_vertex = static_cast<int>(startingPointArrayIndex + faceBorderIndex[0]);
                ei.end_vertex = static_cast<int>(startingPointArrayIndex + faceBorderIndex[1]);

                edge_data_int.push_back(ei);
                edge_data_float.push_back(ef);
            }

            // add face points to pointArray
            for (int i = 0; i < facePointArray.size(); i++)
            {
                OdGePoint3d pointInFacePointArray = facePointArray[i];
                std::array<float, 2> textureUV = faceTextureArray[i];

                pointArray.append(pointInFacePointArray);
                normalArray.append(normal);
                textureArray.push_back(textureUV);
            }

            face = face->next();
        }

    }


    // Create local mesh
    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        // Vertex v = {
        //     {point.x, point.y, point.z},
        //     {normal.x, normal.y, normal.z},
        //     {textureUV[0], textureUV[1]},
        //     OpenGLMaterial::IVORY,
        //     textureIndex
        // };

        vertices_position.push_back({
            static_cast<float>(point.x),
            static_cast<float>(point.y),
            static_cast<float>(point.z),
            0.0f
        });
        vertices_normal.push_back({
            static_cast<float>(normal.x),
            static_cast<float>(normal.y),
            static_cast<float>(normal.z)
        });
        vertices_textureuv.push_back({textureUV[0], textureUV[1]});
        vertices_materialIndex.push_back(materialIndex);
        vertices_textureIndex.push_back(textureIndex);

        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }


    //File Writitng start
    // QString filePath = "D://Meshfile_1.txt";
    // QFile file(filePath);
    // if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
    //     QTextStream out(&file);
    //     for (int i = 0; i < pointArray.size(); i++)
    //     {
    //         OdGePoint3d point = pointArray[i];
    //         out<<point.x;
    //         out<<", ";

    //         out<<point.y;
    //         out<<", ";

    //         out<<point.z;
    //         out<<"\n";
    //     }
    //     file.close();
    // }
    // else
    // {
    //     qDebug() << "Could not open file for writing:" << file.errorString();
    // }
    //End of file writting
}

void OpenglHelper::getMeshGeometry(
    BODY *body,
    std::vector<Position> &vertices_position,
    std::vector<Normal> &vertices_normal,
    std::vector<TextureUV> &vertices_textureuv,
    std::vector<int> &vertices_materialIndex,
    std::vector<int> &vertices_textureIndex,
    std::vector<uint32_t> &meshIndices,
    std::vector<int> &edge_indices,
    std::vector<EdgeDataInt> &edge_data_int,
    std::vector<EdgeDataFloat> &edge_data_float,
    int textureIndex,
    int materialIndex,
    int scalingFactor,
    float edgeWidth,
    float edgeDashLength,
    float edgeGapLength,
    int edgeDash,
    int edgeMaterialIndex
)
{      
    // facet code
    api_facet_entity(body);

    ENTITY_LIST faces;
    api_get_faces(body, faces);

    // qInfo() << "Face count: " << faces.iteration_count();

    faces.init();
    for (int i = 0; i < faces.iteration_count(); i++)
    {
        // qInfo() << "New face";
        ENTITY* itr = faces.next();

        std::vector<float> coords;
        std::vector<int> triangles;
        std::vector<float> normal_coords;
        std::vector<float> uv_coords;

        af_serializable_mesh* sm = GetSerializableMesh((FACE*)itr);
        if (sm == NULL)
        {
            continue;
        }

        const int nv = sm->number_of_vertices();
        int ntri = sm->number_of_polygons();

        coords.resize(3 * nv);
        sm->serialize_positions(coords.data());

        bool const has_normals = sm->has_normals() == TRUE;
        if (has_normals)
        {
            normal_coords.resize(3 * nv);
        }
        sm->serialize_normals(normal_coords.data());

        triangles.resize(3 * ntri);
        int ntri_actual = sm->serialize_triangles(triangles.data());
        while (ntri_actual < ntri)
        {
            triangles.pop_back();
            ntri_actual = static_cast<int>(triangles.size());
        }

        bool const has_uvs = sm->has_uv() == TRUE;
        if (has_uvs)
        {
            uv_coords.resize(2 * nv);
        }
        sm->serialize_uv_data(uv_coords.data(), true);


        int numOfVertices = vertices_position.size();

        for (int i = 0; i < coords.size(); i = i + 3)
        {
            // qInfo() << "Coords: (" << coords[i] << ", " << coords[i + 1] << ", " << coords[i + 2] << ")";
            vertices_position.push_back({ coords[i], coords[i + 1], coords[i + 2], 0.0f }); // position is vec4

            vertices_materialIndex.push_back(materialIndex);
            vertices_textureIndex.push_back(textureIndex);
        }

        for (int i = 0; i < normal_coords.size(); i = i + 3)
        {
            // qInfo() << "Normals: (" << normal_coords[i] << ", " << normal_coords[i + 1] << ", " << normal_coords[i + 2] << ")";
            vertices_normal.push_back({ normal_coords[i], normal_coords[i + 1], normal_coords[i + 2] });
        }

        for (int i = 0; i < uv_coords.size(); i = i + 2)
        {
            // qInfo() << "UVs: (" << uv_coords[i] << ", " << uv_coords[i + 1] << ")";
            vertices_textureuv.push_back({uv_coords[i], uv_coords[i + 1]});
        }

        for (int i = 0; i < triangles.size(); i = i + 3)
        {
            // qInfo() << "Triangles: (" << triangles[i] << ", " << triangles[i + 1] << ", " << triangles[i + 2] << ")";
            meshIndices.push_back(triangles[i] + numOfVertices);
            meshIndices.push_back(triangles[i + 1] + numOfVertices);
            meshIndices.push_back(triangles[i + 2] + numOfVertices);
        }

        // calculate edge data
        FACE* face_itr = (FACE*)itr;
        LOOP* loop = face_itr->loop();
        const LOOP* first_loop = loop;

        do
        {
            // get co-edges in loop
            COEDGE* coedge = loop->start();
            COEDGE* first_coedge = coedge;

            do
            {
                EDGE* edge = coedge->edge();

                std::vector<int> edge_pos_index_array = {};
                SPAposition* pos_array = nullptr;
                int numOfEdgeVertices = 0;
                api_get_facet_edge_points(edge, pos_array, numOfEdgeVertices);

                for (int i = 0; i < numOfEdgeVertices; i++)
                {
                    SPAposition& pos = pos_array[i];
                    // qInfo() << "Position of edge: " << pos.x() << ", " << pos.y() << ", " << pos.z();

                    // get index of that vertices
                    int pos_index = -1;
                    for (int j = numOfVertices; j < vertices_position.size(); j++)
                    {
                        if (
                            (vertices_position[j][0] == pos.x()) &&
                            (vertices_position[j][1] == pos.y()) &&
                            (vertices_position[j][2] == pos.z())
                        )
                        {
                            pos_index = j;
                            break;
                        }
                    }

                    if (pos_index == -1)
                    {
                        // qInfo() << "*****************pos_index == -1";
                        vertices_position.push_back({
                            static_cast<float>(pos.x()),
                            static_cast<float>(pos.y()),
                            static_cast<float>(pos.z()),
                            0.0f
                        }); // position is vec4
                        vertices_textureuv.push_back({0.0f, 0.0f}); // texture uv does not matter

                        SPAunit_vector face_normal = sg_get_face_normal(face_itr, pos);
                        vertices_normal.push_back({
                            static_cast<float>(face_normal.x()),
                            static_cast<float>(face_normal.y()),
                            static_cast<float>(face_normal.z())
                        });

                        vertices_materialIndex.push_back(materialIndex);
                        vertices_textureIndex.push_back(textureIndex);

                        pos_index = vertices_position.size() - 1;
                    }

                    edge_pos_index_array.push_back(pos_index);
                }

                for (int i = 0; i < edge_pos_index_array.size(); i++)
                {
                    if (i == edge_pos_index_array.size() - 1)
                    {
                        break;
                    }
                    else
                    {
                        edge_indices.push_back(edge_indices.size());
                        EdgeDataInt ei;
                        EdgeDataFloat ef;

                        ei.material_index = edgeMaterialIndex;
                        ei.dash = edgeDash;
                        ef.width = edgeWidth;
                        ef.dash_length = edgeDashLength;
                        ef.gap_length = edgeGapLength;
                        ef.padding = 0.0f;

                        ei.start_vertex = static_cast<int>(edge_pos_index_array[i]);
                        ei.end_vertex = static_cast<int>(edge_pos_index_array[i + 1]);

                        edge_data_int.push_back(ei);
                        edge_data_float.push_back(ef);
                    }
                }

                coedge = coedge->next();
            }
            while (coedge != first_coedge);

            loop = loop->next();
        }
        while ((loop != first_loop) && (loop != nullptr));
    }

    // delete entity list
    api_del_entity_list(faces);
}


void OpenglHelper::getMeshGeometry(
    const OdMdBody& body,
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
)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    //FacetModeler::FaceIterator itf(body);
    //itf.get()->normal();
    /*
    FacetModeler::Face* face = body.faceList();
    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
   */

    // for (int i = 0; i < pointArray.size(); i++)
    // {
    //     OdGePoint3d point = pointArray[i];
    //     OdGeVector3d normal = normalArray[i];
    //     std::array<float, 2> textureUV = textureArray[i];

    //     Vertex v = {
    //         {point.x, point.y, point.z},
    //         {normal.x, normal.y, normal.z},
    //         {textureUV[0], textureUV[1]},
    //         0,
    //         textureIndex
    //     };

    //     verticesVector.push_back(v);
    //     // OdGePoint3d point = pointArray[i];
    //     // verticesVector.push_back(point.x); // x
    //     // verticesVector.push_back(point.y); // y
    //     // verticesVector.push_back(point.z); // z

    //     // OdGeVector3d normal = normalArray[i];
    //     // verticesVector.push_back(normal.x); // n.x
    //     // verticesVector.push_back(normal.y); // n.y
    //     // verticesVector.push_back(normal.z); // n.z
    // }
}

void OpenglHelper::getMeshGeometry(
    const BODY& body,
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
)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    //FacetModeler::FaceIterator itf(body);
    //itf.get()->normal();

    /*
    FacetModeler::Face* face = body.faceList();
    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
    */

    // for (int i = 0; i < pointArray.size(); i++)
    // {
    //     OdGePoint3d point = pointArray[i];
    //     OdGeVector3d normal = normalArray[i];
    //     std::array<float, 2> textureUV = textureArray[i];

    //     Vertex v = {
    //         {point.x, point.y, point.z},
    //         {normal.x, normal.y, normal.z},
    //         {textureUV[0], textureUV[1]},
    //         0,
    //         textureIndex
    //     };

    //     verticesVector.push_back(v);
    //     // OdGePoint3d point = pointArray[i];
    //     // verticesVector.push_back(point.x); // x
    //     // verticesVector.push_back(point.y); // y
    //     // verticesVector.push_back(point.z); // z

    //     // OdGeVector3d normal = normalArray[i];
    //     // verticesVector.push_back(normal.x); // n.x
    //     // verticesVector.push_back(normal.y); // n.y
    //     // verticesVector.push_back(normal.z); // n.z
    // }
}

void OpenglHelper::getMeshGeometry(
    const OdBrBrep& brep,
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
)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    /*
    FacetModeler::Face* face = body.faceList();

    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
*/

    // for (int i = 0; i < pointArray.size(); i++)
    // {
    //     OdGePoint3d point = pointArray[i];
    //     OdGeVector3d normal = normalArray[i];
    //     std::array<float, 2> textureUV = textureArray[i];

    //     Vertex v = {
    //         {point.x, point.y, point.z},
    //         {normal.x, normal.y, normal.z},
    //         {textureUV[0], textureUV[1]},
    //         0,
    //         textureIndex
    //     };

    //     verticesVector.push_back(v);
    //     // OdGePoint3d point = pointArray[i];
    //     // verticesVector.push_back(point.x); // x
    //     // verticesVector.push_back(point.y); // y
    //     // verticesVector.push_back(point.z); // z

    //     // OdGeVector3d normal = normalArray[i];
    //     // verticesVector.push_back(normal.x); // n.x
    //     // verticesVector.push_back(normal.y); // n.y
    //     // verticesVector.push_back(normal.z); // n.z
    // }
}

