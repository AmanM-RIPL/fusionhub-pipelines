#include "wall_geometry_service.h"

WallGeometryService::WallGeometryService(QObject *parent)
    : QObject{parent}
{}


void WallGeometryService::generateMesh2D(BIMElement *wallElement)
{
    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    // 1. Find Reference Line parameter and convert to a list of list (2D)

    // 2. Find Width
    QString widthString = "0";
    QString referenceLineString = "[]";

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            referenceLineString = parameter->getValue();
        }
        else if (parameter->getValue() == "Width")
        {
            widthString = parameter->getValue();
        }
    }

    bool ok;
    float width = widthString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};

    QJsonDocument jsonDoc = QJsonDocument::fromJson(referenceLineString.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& outerValue : jsonArray)
    {
        QJsonArray innerArray = outerValue.toArray();
        Point innerList = {innerArray.at(0).toDouble(), innerArray.at(1).toDouble()};
        referenceLine.push_back(innerList);
    }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = generateParallelCurve(referenceLine, width);
    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // 4. Get triangulated mesh
    polygon.push_back(referenceLine);
    polygon.push_back({}); // for holes
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    for (uint32_t index: indices)
    {
        qInfo() << index;
    }
}

std::vector<Point> WallGeometryService::generateParallelCurve(std::vector<Point> referenceCurve, float width)
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
            Point intersectionPoint = getIntersectionPoint(line1, line2);

            parallelCurve.insert(parallelCurve.begin(), intersectionPoint);
        }
    }

    // Get last point
    Point lastPoint = getParallelProjectionPoint(referenceCurve.back(), referenceCurve[referenceCurve.size() - 2], width);
    parallelCurve.insert(parallelCurve.begin(), lastPoint);

    return parallelCurve;
}

Line WallGeometryService::getParallelLineEquation(Point point1, Point point2, float width)
{
    float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
    float b = point1[1] - point1[0] * m;
    float b_parallel = b + (width * qSqrt(qPow(m,2) + 1));

    Line result = {m, b_parallel};

    return result;
}

Point WallGeometryService::getIntersectionPoint(Line line1, Line line2)
{
    float x = (line2[1] - line1[1]) / (line1[0] - line2[0]);
    float y = line1[0] * x + line1[1];

    Point result = {x, y};

    return result;
}

Point WallGeometryService::getParallelProjectionPoint(Point point1, Point point2, float width)
{
    float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
    float x_proj = point1[0] + width * (-m/qSqrt(qPow(m,2) + 1));
    float y_proj = point1[1] + width * (1/qSqrt(qPow(m,2) + 1));

    Point result = {x_proj, y_proj};

    return result;
}
