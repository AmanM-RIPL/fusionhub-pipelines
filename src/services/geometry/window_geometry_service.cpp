#include "window_geometry_service.h"

WindowGeometryService::WindowGeometryService(QObject *parent)
    : QObject{parent}
{}

void WindowGeometryService::generateMesh2D(BIMElement* windowElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(windowElement, referenceLine, width, height, distance);

    /*

    x = (x2 - x1)/sqrt((x2 - x1)^2 + (y2 - y1)^2) * widthDoor + x1
    y = (y2 - y1)/sqrt((x2 - x1)^2 + (y2 - y1)^2) * widthDoor + y1

    */

    float windowWidthPointX = (((referenceLine[1][0] - referenceLine[0][0])/(qSqrt(qPow(referenceLine[1][0] - referenceLine[0][0], 2) + qPow(referenceLine[1][1] - referenceLine[0][1], 2))))*width) + referenceLine[0][0];
    float windowWidthPointY = (((referenceLine[1][1] - referenceLine[0][1])/(qSqrt(qPow(referenceLine[1][0] - referenceLine[0][0], 2) + qPow(referenceLine[1][1] - referenceLine[0][1], 2))))*width) + referenceLine[0][1];

    referenceLine[1][0] = windowWidthPointX;
    referenceLine[1][1] = windowWidthPointY;

    // setting width to 0.5 for demo purposes
    width = 0.5;

    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
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
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        Point point = referenceLine[i];

        // Vertex v = {
        //     {point[0], point[1], 0.0f},
        //     {0.0f, 0.0f, 1.0f},
        //     {0.0f, 0.0f},
        //     OpenGLMaterial::IVORY,
        //     Texture::NONE
        // };

        vertices_position.push_back({point[0], point[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        // verticesVector.push_back(point[0]); // x
        // verticesVector.push_back(point[1]); // y
        // verticesVector.push_back(0.0f); // z
        // verticesVector.push_back(0.0f); // n.x
        // verticesVector.push_back(0.0f); // n.y
        // verticesVector.push_back(1.0f); // n.z
    }

    std::vector<EdgeIndex> edge_indices = {};
    std::vector<float> edge_width = {};
    std::vector<int> edge_materialIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        edge_width.push_back(1);
        edge_materialIndex.push_back(OpenGLMaterial::BLACK);

        if (i == referenceLine.size() - 1)
        {
            edge_indices.push_back({i, 0});
        }
        else
        {
            edge_indices.push_back({i, i + 1});
        }
    }

    // // Allocate memory for the new array using std::unique_ptr for safety.
    // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // Copy elements from the vector to the new array.
    // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_indices,
        edge_width,
        edge_materialIndex,
        indices
        );
    mesh->setBIMElementId(windowElement->getId());


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

void WindowGeometryService::generateMesh3D(BIMElement* windowElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(windowElement, referenceLine, width, height, distance);

    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    // Initialize the mesh
    mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {});

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    // set scale
    float scaleFactor = height/4; // height of IFC file is 0.45
    modelMatrix.scale(scaleFactor);

    // set translation
    float z = distance + 1;
    float x = referenceLine[0][0] + 1.5; // 0.0 is the default x coordinate of left side
    float y = referenceLine[0][1] - 0; // 0.17 is the default y coordinate of left size
    modelMatrix.translate(x,y,z);

    // set rotation
    QVector3D directionVector;
    directionVector.setX(referenceLine[1][0] - referenceLine[0][0]);
    directionVector.setY(referenceLine[1][1] - referenceLine[0][1]);
    directionVector.setZ(0);
    directionVector.normalize();

    QVector3D xAxisVector(1,0,0);
    float dotProductResult = QVector3D::dotProduct(directionVector,xAxisVector);
    float angleInRadians = qAcos(dotProductResult);
    float degrees = qRadiansToDegrees(angleInRadians);

    modelMatrix.rotate(degrees, 0, 0, 1);

    mesh->setModelMatrix(modelMatrix);
    mesh->setBIMElementId(windowElement->getId());




    // Extract IFC Geometry
    QString strFilePath = "C:\\Users\\RIPL\\Downloads\\sample_window.ifc";

    OdIfcFilePtr pDatabase = pIfcDetailController->getIfcFilePtrFromLoadedIFC(strFilePath);
    if(pDatabase)
    {
        pIfcGeometryService->generateMesh3D(pDatabase, mesh);
        qInfo() << "File has been loaded";
        pDatabase.release();
        pDatabase = NULL;
    }

    // // 3. Generate a parallel line
    // std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    // referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // // Create a contour2D
    // FacetModeler::Contour2D polygon;

    // OdGePoint2dArray points;
    // points.reserve(referenceLine.size());

    // for (Point point: referenceLine)
    // {
    //     points.push_back(OdGePoint2d(point[0], point[1]));
    // }

    // polygon.appendVertices(points);

    // for (int i = 0; i < referenceLine.size(); i++)
    // {
    //     polygon.setOrientationAt(i, FacetModeler::efoFront);
    // }

    // polygon.setClosed();
    // polygon.makeCCW();

    // FacetModeler::Profile2D profile(polygon);
    // FacetModeler::Body body = FacetModeler::Body::extrusion(profile, OdGeVector3d(0.0, 0.0, 1.0) * height);

    // // Mesh geometry generation
    // std::vector<uint32_t> meshIndices = {};
    // std::vector<uint32_t> borderIndices = {};
    // std::vector<Vertex> verticesVector = {};
    // int textureIndex = 0; // if less than zero then we don't need to worry about textures
    // int scalingFactor = 5;

    // m_openglHelper.getMeshGeometry(body, verticesVector, meshIndices, borderIndices, textureIndex, scalingFactor);

    // mesh->Initialize(verticesVector, meshIndices, borderIndices, verticesVector.size(), meshIndices.size(), borderIndices.size());
}

FacetModeler::Body WindowGeometryService::generateVoidBody(BIMElement *windowElement, BIMElement* hostElement)
{
    // Window Element Parsing
    std::vector<Point> referenceLineWindow = {};
    float widthWindow = 0;
    float heightWindow = 0;
    float distanceWindow = 0;


    m_openglHelper.extractBIMParameters(windowElement, referenceLineWindow, widthWindow, heightWindow, distanceWindow);

    // if (referenceLineWindow.size() < 2)
    // {
    //     return;
    // }

    /*

    x = (x2 - x1)/sqrt((x2 - x1)^2 + (y2 - y1)^2) * widthWindow + x1
    y = (y2 - y1)/sqrt((x2 - x1)^2 + (y2 - y1)^2) * widthWindow + y1

    */

    float windowWidthPointX = (((referenceLineWindow[1][0] - referenceLineWindow[0][0])/(qSqrt(qPow(referenceLineWindow[1][0] - referenceLineWindow[0][0], 2) + qPow(referenceLineWindow[1][1] - referenceLineWindow[0][1], 2))))*widthWindow) + referenceLineWindow[0][0];
    float windowWidthPointY = (((referenceLineWindow[1][1] - referenceLineWindow[0][1])/(qSqrt(qPow(referenceLineWindow[1][0] - referenceLineWindow[0][0], 2) + qPow(referenceLineWindow[1][1] - referenceLineWindow[0][1], 2))))*widthWindow) + referenceLineWindow[0][1];

    referenceLineWindow[1][0] = windowWidthPointX;
    referenceLineWindow[1][1] = windowWidthPointY;

    // Host Element Parsing
    std::vector<Point> referenceLineHost = {};
    float widthHost = 0;
    float heightHost = 0;
    float distanceHost = 0;


    m_openglHelper.extractBIMParameters(hostElement, referenceLineHost, widthHost, heightHost, distanceHost);

    // if (referenceLineHost.size() < 2)
    // {
    //     return;
    // }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLineWindow, widthHost);

    referenceLineWindow.insert(referenceLineWindow.end(), parallelLine.begin(), parallelLine.end());

    //Create a contour2D
    FacetModeler::Contour2D polygon;

    OdGePoint2dArray points;
    points.reserve(referenceLineWindow.size());

    for (Point point: referenceLineWindow)
    {
        points.push_back(OdGePoint2d(point[0], point[1]));
    }

    polygon.appendVertices(points);

    for (int i = 0; i < referenceLineWindow.size(); i++)
    {
        polygon.setOrientationAt(i, FacetModeler::efoFront);
    }

    polygon.setClosed();
    polygon.makeCCW();

    FacetModeler::Profile2D profile(polygon);
    FacetModeler::Body body = FacetModeler::Body::extrusion(profile, OdGeVector3d(0.0, 0.0, 1.0) * heightWindow);

    OdGeVector3d translationVector(0.0, 0.0, distanceWindow);
    OdGeMatrix3d matrix;
    matrix.setToTranslation(translationVector);

    body.transform(matrix);

    return body;
}

void WindowGeometryService::updateGeometry(BIMElement *windowElement, BIMElement* hostElement, const QVector3D &point)
{
    // Window Element Parsing
    std::vector<Point> referenceLineWindow = {};
    float widthWindow = 0;
    float heightWindow = 0;
    float distanceWindow = 0;


    m_openglHelper.extractBIMParameters(windowElement, referenceLineWindow, widthWindow, heightWindow, distanceWindow);

    // Host Element Parsing
    std::vector<Point> referenceLineHost = {};
    float widthHost = 0;
    float heightHost = 0;
    float distanceHost = 0;


    m_openglHelper.extractBIMParameters(hostElement, referenceLineHost, widthHost, heightHost, distanceHost);

    // window's host element can only be a wall
    if (hostElement->getType() != "Wall")
    {
        return;
    }

    // We project the clicked point on the each reference line segment
    // then we choose the segment with the smalled distance to that line
    // segment. The second point of that segment is the line segment we need.
    float distanceToSegment = 0;
    Point projectionPointOnSegment = { 0.0f, 0.0f };
    int referenceLineHostIndex = 0;

    for (int i = 0; i < referenceLineHost.size() - 1; i++)
    {
        Point firstPoint = referenceLineHost[i];
        Point secondPoint = referenceLineHost[i + 1];

        // vector from firstPoint to secondPoint
        QVector3D firstPointVector = QVector3D(firstPoint[0], firstPoint[1], 0.0f);
        QVector3D secondPointVector = QVector3D(secondPoint[0], secondPoint[1], 0.0f);
        QVector3D vectorReferenceLine = secondPointVector - firstPointVector;

        // vector deom firstPoint to clicked point
        QVector3D vectorToClickedPoint = point - firstPointVector;

        // projection point
        QVector3D projectionPoint = firstPointVector + (QVector3D::dotProduct(vectorToClickedPoint, vectorReferenceLine)/QVector3D::dotProduct(vectorReferenceLine, vectorReferenceLine)) * vectorReferenceLine;

        // distance between projection point and clicked point
        float distanceToProjection = projectionPoint.distanceToPoint(point);

        if (i == 0)
        {
            distanceToSegment = distanceToProjection;
            projectionPointOnSegment[0] = projectionPoint.x();
            projectionPointOnSegment[1] = projectionPoint.y();
            referenceLineHostIndex = i;
        }
        else if (distanceToProjection < distanceToSegment)
        {
            distanceToSegment = distanceToProjection;
            projectionPointOnSegment[0] = projectionPoint.x();
            projectionPointOnSegment[1] = projectionPoint.y();
            referenceLineHostIndex = i;
        }
    }

    // ideally should indicate some kind of bug, please check.
    if (referenceLineHostIndex < 0)
    {
        return;
    }

    // update the new point in the reference line
    referenceLineWindow.push_back({ projectionPointOnSegment[0], projectionPointOnSegment[1] });
    referenceLineWindow.push_back({ referenceLineHost[referenceLineHostIndex + 1][0], referenceLineHost[referenceLineHostIndex + 1][1] });

    // updating the BIMElement
    QJsonArray referenceLineJsonArray;

    for (const auto& pointArray : referenceLineWindow) {
        QJsonArray jsonInnerArray;
        for (float value : pointArray) {
            jsonInnerArray.append(QJsonValue(value));
        }
        referenceLineJsonArray.append(jsonInnerArray);
    }

    QJsonDocument jsonDoc(referenceLineJsonArray);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    QString referenceLineString = QString(byteArray);

    QList<BIMParameter*> parameterList = windowElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            parameter->setValue(referenceLineString);

            break;
        }
    }

    // updatin host_id in BIMElement
    windowElement->setHostId(hostElement->getId());
}
