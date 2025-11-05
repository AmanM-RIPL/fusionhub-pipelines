#include "opengl_helper.h"

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"


/*OpenglHelper::OpenglHelper(QObject *parent)
    : QObject{parent}
{}*/

OpenglHelper::OpenglHelper()
{}

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

    QJsonDocument jsonDoc = QJsonDocument::fromJson(referenceLineString.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& outerValue : jsonArray)
    {
        QJsonArray innerArray = outerValue.toArray();
        Point innerList = {innerArray.at(0).toDouble(), innerArray.at(1).toDouble()};
        referenceLine.push_back(innerList);
    }

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

void OpenglHelper::getMeshGeometry(const FacetModeler::Body& body, OdGePoint3dArray& pointArray, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, OdGeVector3dArray& normalArray)
{
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
}
