#include "wall_geometry_service.h"

WallGeometryService::WallGeometryService(QObject *parent)
    : QObject{parent}
{}


void WallGeometryService::generateMesh2D(BIMElement *wallElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;

    extractBIMParameters(wallElement, referenceLine, width, height);

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // for (Point point: referenceLine)
    // {
    //     qInfo() << "x: " << point[0] << " , y: " << point[1];
    // }

    // 4. Get triangulated mesh
    polygon.push_back(referenceLine);
    polygon.push_back({}); // for holes
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // for (uint32_t index: indices)
    // {
    //     qInfo() << index;
    // }

    // 5. Create and export mesh
    std::vector<Vertex> verticesVector = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        Point point = referenceLine[i];

        Vertex v = {
            {point[0], point[1], 0.0f},
            {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f},
            0,
            -1
        };

        // verticesVector.push_back(point[0]); // x
        // verticesVector.push_back(point[1]); // y
        // verticesVector.push_back(0.0f); // z
        // verticesVector.push_back(0.0f); // n.x
        // verticesVector.push_back(0.0f); // n.y
        // verticesVector.push_back(1.0f); // n.z
    }

    std::vector<uint32_t> borderIndices = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        borderIndices.push_back(i);

        if (i == referenceLine.size() - 1)
        {
            borderIndices.push_back(0);
        }
        else
        {
            borderIndices.push_back(i + 1);
        }
    }

    // // Allocate memory for the new array using std::unique_ptr for safety.
    // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // Copy elements from the vector to the new array.
    // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
    mesh->Initialize(verticesVector, indices, borderIndices, referenceLine.size(), indices.size(), borderIndices.size());


    // GLfloat* vertices1 = mesh->getVerticies();
    // unsigned int* indices1 = mesh->getIndices();
    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << vertices1[6*i] << " , " << vertices1[6*i + 1] << " , " << vertices1[6*i + 2] << " , " << vertices1[6*i + 3] << " , " << vertices1[6*i + 4] << " , " << vertices1[6*i + 5];
    // }

    // qInfo() << "----------------------------------";

    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << indices1[i];
    // }

    // return mesh;
}

void WallGeometryService::generateMesh3D(BIMElement *wallElement, Mesh* mesh)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;

    extractBIMParameters(wallElement, referenceLine, width, height);

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // Create a contour2D
    FacetModeler::Contour2D polygon;

    OdGePoint2dArray points;
    points.reserve(referenceLine.size());

    for (Point point: referenceLine)
    {
        points.push_back(OdGePoint2d(point[0], point[1]));
    }

    polygon.appendVertices(points);

    for (int i = 0; i < referenceLine.size(); i++)
    {
        polygon.setOrientationAt(i, FacetModeler::efoFront);
    }

    polygon.setClosed();
    polygon.makeCCW();

    FacetModeler::Profile2D profile(polygon);
    FacetModeler::Body body = FacetModeler::Body::extrusion(profile, OdGeVector3d(0.0, 0.0, 1.0) * height);


    // Mesh geometry generation
    OdGePoint3dArray pointArray = {};
    std::vector<uint32_t> meshIndices = {};
    std::vector<uint32_t> borderIndices = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};
    int textureIndex = 0; // if less than zero then we don't need to worry about textures
    int scalingFactor = 5;

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
    else
    {
        FacetModeler::Face* face = body.faceList();
        for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
        {
            OdGePoint3dArray facePointArray = {};
            std::vector<std::array<float, 2>> faceTextureArray = {};
            std::vector<uint32_t> faceBorderIndices = {};
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
                int startingFacePointIndex = facePointArray.size();
                for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
                {
                    facePointArray.append(edge->startPoint());
                    faceBorderIndices.push_back(facePointArray.size() - 1); // for first point

                    if (edgeIndex < face->loopEdgeCount(loopIndex) - 1)
                    {
                        faceBorderIndices.push_back(facePointArray.size()); // for second point
                    }
                    else
                    {
                        faceBorderIndices.push_back(startingFacePointIndex); // for last point the end point is first point
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

            for (int faceBorderIndex: faceBorderIndices)
            {
                borderIndices.push_back(startingPointArrayIndex + faceBorderIndex);
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
    std::vector<Vertex> verticesVector = {};
    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        Vertex v = {
            {point.x, point.y, point.z},
            {normal.x, normal.y, normal.z},
            {textureUV[0], textureUV[1]},
            0,
            textureIndex
        };

        verticesVector.push_back(v);

        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }

    mesh->Initialize(verticesVector, meshIndices, borderIndices, verticesVector.size(), meshIndices.size(), borderIndices.size());
}

void WallGeometryService::extractBIMParameters(BIMElement *wallElement, std::vector<Point> &referenceLine, float &width, float &height)
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
        else if (parameter->getKey() == "Width")
        {
            widthString = parameter->getValue();
        }
    }

    bool ok;
    width = widthString.toFloat(&ok);
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

    height = 4;
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

Line WallGeometryService::getParallelLineEquation(Point point1, Point point2, float width)
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

Point WallGeometryService::getIntersectionPoint(Line line1, Line line2)
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

Point WallGeometryService::getParallelProjectionPoint(Point point1, Point point2, float width)
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
