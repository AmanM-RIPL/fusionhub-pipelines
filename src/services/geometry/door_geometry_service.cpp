#include "door_geometry_service.h"

DoorGeometryService::DoorGeometryService(QObject *parent)
    : QObject{parent}
{}

void DoorGeometryService::generateMesh2D(BIMElement* doorElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(doorElement, referenceLine, width, height, distance);

    // setting width to 0.5 for demo purposes
    width = 0.5;

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

        // Point point = referenceLine[i];
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

void DoorGeometryService::generateMesh3D(BIMElement* doorElement, Mesh* mesh, IFCDetailController* pIfcDetailController, IfcGeometryService* pIfcGeometryService)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(doorElement, referenceLine, width, height, distance);

    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, 0, 0, 0);
        return;
    }

    // Initialize the mesh
    mesh->Initialize({}, {}, {}, 0, 0, 0);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    // set scale
    float scaleFactor = height/6; // height of IFC file is 6
    modelMatrix.scale(scaleFactor);

    // // set translation
    // float z = distance;
    // float x = referenceLine[0][0] - 2; // 2 is the default x coordinate of left side
    // float y = referenceLine[0][1] - 0.5; // 0.5 is the default y coordinate of left size
    // modelMatrix.translate(x,y,z);

    // // set rotation
    // QVector3D directionVector;
    // directionVector.setX(referenceLine[1][0] - referenceLine[0][0]);
    // directionVector.setY(referenceLine[1][1] - referenceLine[0][1]);
    // directionVector.setZ(0);
    // directionVector.normalize();

    // QVector3D xAxisVector(1,0,0);
    // float dotProductResult = QVector3D::dotProduct(directionVector,xAxisVector);
    // float angleInRadians = qAcos(dotProductResult);
    // float degrees = qRadiansToDegrees(angleInRadians);

    // modelMatrix.rotate(degrees, 1, 0, 0);

    mesh->setModelMatrix(modelMatrix);




    // Extract IFC Geometry
    QString strFilePath = "C:\\Users\\RIPL\\Downloads\\DblDoor-1-Panel.ifc";

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

