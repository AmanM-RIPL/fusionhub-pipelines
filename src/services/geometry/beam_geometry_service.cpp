#include "beam_geometry_service.h"

BeamGeometryService::BeamGeometryService(QObject *parent)
    : QObject{parent}
{}

void BeamGeometryService::generateMesh2D(BIMElement* beamElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(beamElement, referenceLine, width, height, distance);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {});
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
    std::vector<float> edge_dashLength = {};
    std::vector<float> edge_gapLength = {};
    std::vector<int> edge_dash = {};
    std::vector<int> edge_materialIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        edge_width.push_back(1.0f);
        edge_dashLength.push_back(1.0f);
        edge_gapLength.push_back(1.0f);
        edge_dash.push_back(0);
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
        edge_dashLength,
        edge_gapLength,
        edge_dash,
        edge_materialIndex,
        indices
    );
    mesh->setBIMElementId(beamElement->getId());


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

void BeamGeometryService::generateMesh3D(BIMElement* beamElement, Mesh* mesh)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(beamElement, referenceLine, width, height, distance);

    // if reference line is only one point then we don't need to render
    if (referenceLine.size() < 2)
    {
        mesh->Initialize({}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {});
        return;
    }

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    //Create a contour2D
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
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<EdgeIndex> edge_indices = {};
    std::vector<float> edge_width = {};
    std::vector<float> edge_dashLength = {};
    std::vector<float> edge_gapLength = {};
    std::vector<int> edge_dash = {};
    std::vector<int> edge_materialIndex = {};
    int textureIndex = Texture::BRICK; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1.0f;
    float edgeDashLength = 1.0f;
    float edgeGapLength = 1.0f;
    int edgeDash = 0;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;

    m_openglHelper.getMeshGeometry(
        body,
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        meshIndices,
        edge_indices,
        edge_width,
        edge_dashLength,
        edge_gapLength,
        edge_dash,
        edge_materialIndex,
        textureIndex,
        materialIndex,
        scalingFactor,
        edgeWidth,
        edgeDashLength,
        edgeGapLength,
        edgeDash,
        edgeMaterialIndex
    );

    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_indices,
        edge_width,
        edge_dashLength,
        edge_gapLength,
        edge_dash,
        edge_materialIndex,
        meshIndices
    );
    mesh->setBIMElementId(beamElement->getId());
}

void BeamGeometryService::updateGeometry(BIMElement *beamElement, const QVector3D &point)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;


    m_openglHelper.extractBIMParameters(beamElement, referenceLine, width, height, distance);

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

    QList<BIMParameter*> parameterList = beamElement->getParameterList();

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            parameter->setValue(referenceLineString);

            break;
        }
    }
}
