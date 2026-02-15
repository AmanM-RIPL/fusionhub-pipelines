#include "stairs_geometry_service.h"

StairsGeometryService::StairsGeometryService(QObject *parent)
    : QObject{parent}
{}


void StairsGeometryService::generateMesh2D(BIMElement* stairsElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(stairsElement, referenceLine, width, height, distance);

    // 3. Generate a parallel line
    //std::vector<Point> parallelLine = generateParallelCurve(referenceLine, width);

    //referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

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

    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        EdgeDataInt ei;
        EdgeDataFloat ef;

        ei.material_index = OpenGLMaterial::BLACK;
        ei.dash = 0;
        ef.width = 1.0f;
        ef.dash_length = 1.0f;
        ef.gap_length = 1.0f;
        ef.padding = 0.0f;

        if (i == referenceLine.size() - 1)
        {
            ei.start_vertex = i;
            ei.end_vertex = 0;
        }
        else
        {
            ei.start_vertex = i;
            ei.end_vertex = i + 1;
        }

        edge_data_int.push_back(ei);
        edge_data_float.push_back(ef);
        edge_indices.push_back(i);
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
        edge_data_int,
        edge_data_float,
        indices,
        edge_indices
    );
    mesh->setBIMElementId(stairsElement->getId());


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

    // return mesh;s
}

void StairsGeometryService::generateMesh3D(BIMElement* stairsElement, Mesh* mesh)
{
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(stairsElement, referenceLine, width, height, distance);

    // 3. Generate a parallel line
    //std::vector<Point> parallelLine = generateParallelCurve(referenceLine, width);
    //referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

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

    //Create a translation matrix to move the body by 5 units in the Z direction
    OdGeMatrix3d translationMatrix;
    OdGeVector3d moveVector(0.0, 0.0, distance);
    translationMatrix.setTranslation(moveVector);

    //Apply the transformation to the body
    body.transform(translationMatrix);

    // Mesh geometry generation
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<int> edge_indices = {};
    std::vector<EdgeDataInt> edge_data_int = {};
    std::vector<EdgeDataFloat> edge_data_float = {};
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
        edge_data_int,
        edge_data_float,
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
        edge_data_int,
        edge_data_float,
        meshIndices,
        edge_indices
    );
}
