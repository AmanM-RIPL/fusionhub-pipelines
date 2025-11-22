#include "wall_geometry_service.h"

WallGeometryService::WallGeometryService(QObject *parent)
    : QObject{parent}
{}


void WallGeometryService::generateMesh2D(BIMElement* wallElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, 0, 0, 0);
        return;
    }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

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

        verticesVector.push_back(v);

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

void WallGeometryService::generateMesh3D(BIMElement* wallElement, Mesh* mesh)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(wallElement, referenceLine, width, height, distance);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, 0, 0, 0);
        return;
    }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

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
    std::vector<GLfloat> verticesVector = {};
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

void WallGeometryService::updateGeometry(BIMElement *wallElement, const QVector3D &point)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;

    extractBIMParameters(wallElement, referenceLine, width, height);

    // update the new point in the reference line
    referenceLine.push_back({ point.x(), point.y() });

    // updating the BIMElement
    QJsonArray referenceLineJsonArray;

    for (const auto& pointArray : referenceLine) {
        QJsonArray jsonInnerArray;
        for (float value : pointArray) {
            jsonInnerArray.append(QJsonValue(value));
        }
        referenceLineJsonArray.append(jsonInnerArray);
    }

    QJsonDocument jsonDoc(referenceLineJsonArray);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    QString referenceLineString = QString(byteArray);

    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            parameter->setValue(referenceLineString);

            break;
        }
    }
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

    m_openglHelper.getMeshGeometry(body, verticesVector, meshIndices, borderIndices);

    mesh->Initialize(verticesVector, meshIndices, borderIndices, verticesVector.size(), meshIndices.size(), borderIndices.size());
}
