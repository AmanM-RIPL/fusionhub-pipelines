#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject{parent}
{}

void Mesh::Initialize(
    const std::vector<Position>& vertices_position,
    const std::vector<Normal>& vertices_normal,
    const std::vector<TextureUV>& vertices_textureuv,
    const std::vector<int>& vertices_materialIndex,
    const std::vector<int>& vertices_textureIndex,

    const std::vector<EdgeDataInt>& edge_data_int,
    const std::vector<EdgeDataFloat>& edge_data_float,

    const std::vector<unsigned int>& indices,
    const std::vector<int>& edge_indices
)
{
    m_verticies_position = vertices_position;
    m_verticies_normal = vertices_normal;
    m_verticies_textureuv = vertices_textureuv;
    m_verticies_materialIndex = vertices_materialIndex;
    m_verticies_textureIndex = vertices_textureIndex;

    m_edge_data_int = edge_data_int;
    m_edge_data_float = edge_data_float;

    m_indices = indices;
    m_edge_indices = edge_indices;

    m_modelMatrix = QMatrix4x4();

    unsigned char r,g,b;
    encodeIdToColor(getBIMElementId(), r,g,b);
    std::array<float, 4> pickColor = { r/255.0f, g/255.0f, b/255.0f, 1.0f };

    std::vector<QMatrix4x4> model_matrix_list = { m_modelMatrix };
    std::vector<std::array<float, 4>> pickColor_array;
    std::vector<int> model_matrix_indices;

    for (Position meshVertex: vertices_position)
    {
        model_matrix_indices.push_back(0); // 0, as model_matrix_list is 0
        pickColor_array.push_back(pickColor);
    }


    SetModelMatricies(model_matrix_list);
    SetModelMatrixIndices(model_matrix_indices);
    SetPickColorArray(pickColor_array);
}

void Mesh::AppendGeometry(
    const std::vector<Position>& vertices_position,
    const std::vector<Normal>& vertices_normal,
    const std::vector<TextureUV>& vertices_textureuv,
    const std::vector<int>& vertices_materialIndex,
    const std::vector<int>& vertices_textureIndex,

    const std::vector<EdgeDataInt>& edge_data_int,
    const std::vector<EdgeDataFloat>& edge_data_float,

    const std::vector<unsigned int>& indices,
    const std::vector<int>& edge_indices
)
{
    // need before new vertices are inserted
    int numOfVertices = m_verticies_position.size();
    int numOfEdges = m_edge_data_int.size();

    m_verticies_position.insert(m_verticies_position.end(), vertices_position.begin(),  vertices_position.end());
    m_verticies_normal.insert(m_verticies_normal.end(), vertices_normal.begin(),  vertices_normal.end());
    m_verticies_textureuv.insert(m_verticies_textureuv.end(), vertices_textureuv.begin(),  vertices_textureuv.end());
    m_verticies_materialIndex.insert(m_verticies_materialIndex.end(), vertices_materialIndex.begin(),  vertices_materialIndex.end());
    m_verticies_textureIndex.insert(m_verticies_textureIndex.end(), vertices_textureIndex.begin(),  vertices_textureIndex.end());

    for (const unsigned int& index: indices)
    {
        m_indices.push_back(index + numOfVertices);
    }

    for (const int& index: edge_indices)
    {
        m_edge_indices.push_back(index + numOfEdges );
    }

    for (const EdgeDataInt& edgeData: edge_data_int)
    {
        EdgeDataInt edgeDataNew;
        edgeDataNew.start_vertex = edgeData.start_vertex + numOfVertices;
        edgeDataNew.end_vertex = edgeData.end_vertex + numOfVertices;
        edgeDataNew.material_index = edgeData.material_index;
        edgeDataNew.dash = edgeData.dash;

        m_edge_data_int.push_back(edgeDataNew);
    }

    m_edge_data_float.insert(m_edge_data_float.end(), edge_data_float.begin(), edge_data_float.end());
}

void Mesh::Copy(Mesh *mesh)
{
    mesh->Initialize(
        m_verticies_position,
        m_verticies_normal,
        m_verticies_textureuv,
        m_verticies_materialIndex,
        m_verticies_textureIndex,
        m_edge_data_int,
        m_edge_data_float,
        m_indices,
        m_edge_indices
    );
}

void Mesh::Combine(Mesh *combinedMesh, QList<Mesh *> meshList)
{
    std::vector<Position> verticies_position;
    std::vector<Normal> verticies_normal;
    std::vector<TextureUV> verticies_textureuv;
    std::vector<int> verticies_materialIndex;
    std::vector<int> verticies_textureIndex;

    std::vector<EdgeDataInt> edge_data_int;
    std::vector<EdgeDataFloat> edge_data_float;

    std::vector<unsigned int> indices;
    std::vector<int> edge_indices;
    std::vector<std::array<float, 4>> pickColor_array;

    std::vector<int> model_matrix_indices;
    std::vector<QMatrix4x4> model_matrix = {};
    QMatrix4x4 firstModelMatrix;
    firstModelMatrix.setToIdentity();
    model_matrix.push_back(firstModelMatrix);

    unsigned int numOfVertices = 0;
    unsigned int numOfIndices = 0;
    unsigned int numOfEdgeIndices = 0;
    unsigned int numOfEdges = 0;

    for (Mesh* mesh: meshList)
    {
        // needed for color picking
        unsigned char r,g,b;
        encodeIdToColor(mesh->getBIMElementId(), r,g,b);
        std::array<float, 4> pickColor = { r/255.0f, g/255.0f, b/255.0f, 1.0f };

        std::vector<Position> meshVerticiesPosition = mesh->getVerticiesPosition();
        std::vector<Normal> meshVerticiesNormal = mesh->getVerticiesNormal();
        std::vector<TextureUV> meshVerticiesTextureUV = mesh->getVerticiesTextureUV();
        std::vector<int> meshVerticiesMaterialIndex = mesh->getVerticiesMaterialIndex();
        std::vector<int> meshVerticiesTextureIndex = mesh->getVerticiesTextureIndex();

        verticies_position.insert(verticies_position.end(), meshVerticiesPosition.begin(),  meshVerticiesPosition.end());
        verticies_normal.insert(verticies_normal.end(), meshVerticiesNormal.begin(),  meshVerticiesNormal.end());
        verticies_textureuv.insert(verticies_textureuv.end(), meshVerticiesTextureUV.begin(),  meshVerticiesTextureUV.end());
        verticies_materialIndex.insert(verticies_materialIndex.end(), meshVerticiesMaterialIndex.begin(),  meshVerticiesMaterialIndex.end());
        verticies_textureIndex.insert(verticies_textureIndex.end(), meshVerticiesTextureIndex.begin(),  meshVerticiesTextureIndex.end());

        unsigned int modelIndex = 0; // 0 for identity matrix
        QMatrix4x4 meshModelMatrix = mesh->getModelMatrix();

        if (!meshModelMatrix.isIdentity())
        {

            // qInfo() << meshModelMatrix(0,0) << ", " << meshModelMatrix(0,1) << ", " << meshModelMatrix(0,2) << ", " << meshModelMatrix(0,3);
            // qInfo() << meshModelMatrix(1,0) << ", " << meshModelMatrix(1,1) << ", " << meshModelMatrix(1,2) << ", " << meshModelMatrix(1,3);
            // qInfo() << meshModelMatrix(2,0) << ", " << meshModelMatrix(2,1) << ", " << meshModelMatrix(2,2) << ", " << meshModelMatrix(2,3);
            // qInfo() << meshModelMatrix(3,0) << ", " << meshModelMatrix(3,1) << ", " << meshModelMatrix(3,2) << ", " << meshModelMatrix(3,3);

            model_matrix.push_back(meshModelMatrix);
            modelIndex = model_matrix.size() - 1; // identity at index 0
        }

        for (Position meshVertex: meshVerticiesPosition)
        {
            model_matrix_indices.push_back(modelIndex);
            pickColor_array.push_back(pickColor);
        }

        std::vector<unsigned int> meshIndices = mesh->getIndices();
        for (unsigned int& index: meshIndices)
        {
            index += numOfVertices;
        }
        indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

        std::vector<int> meshEdgeIndices = mesh->getEdgeIndices();
        for (int& index: meshEdgeIndices)
        {
            index += numOfEdges;
        }
        edge_indices.insert(edge_indices.end(), meshEdgeIndices.begin(), meshEdgeIndices.end());

        std::vector<EdgeDataInt> meshEdgeDataInt = mesh->getEdgeDataInt();
        for (EdgeDataInt& edgeData: meshEdgeDataInt)
        {
            edgeData.start_vertex += numOfVertices;
            edgeData.end_vertex += numOfVertices;
        }
        edge_data_int.insert(edge_data_int.end(), meshEdgeDataInt.begin(), meshEdgeDataInt.end());

        std::vector<EdgeDataFloat> meshEdgeDataFloat = mesh->getEdgeDataFloat();
        edge_data_float.insert(edge_data_float.end(), meshEdgeDataFloat.begin(),  meshEdgeDataFloat.end());

        numOfVertices += mesh->getNumOfVertices();
        numOfIndices += mesh->getNumOfIndices();
        numOfEdges += mesh->getNumOfEdges();
        numOfEdgeIndices += mesh->getNumOfEdgeIndices();
    }


    combinedMesh->Initialize(
        verticies_position,
        verticies_normal,
        verticies_textureuv,
        verticies_materialIndex,
        verticies_textureIndex,
        edge_data_int,
        edge_data_float,
        indices,
        edge_indices
    );
    combinedMesh->SetModelMatricies(model_matrix);
    combinedMesh->SetModelMatrixIndices(model_matrix_indices);
    combinedMesh->SetPickColorArray(pickColor_array);
}

void Mesh::GenerateBaseSurface(Mesh *mesh)
{
    std::vector<Position> verticies_position = {
        {-10.0f, -10.0f, 0.0f, 0.0f},
        {-10.0f, 10.0f, 0.0f, 0.0f},
        {10.0f, 10.0f, 0.0f, 0.0f},
        {10.0f, -10.0f, 0.0f, 0.0f}
    };
    std::vector<Normal> verticies_normal = {
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f}
    };
    std::vector<TextureUV> verticies_textureuv = {
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 0.0f}
    };
    std::vector<int> verticies_materialIndex = {1,1,1,1};
    std::vector<int> verticies_textureIndex = {-1,-1,-1,-1};

    std::vector<unsigned int> indices = {0,2,1, 0,3,2}; // winding order as face culling is switched on
    std::vector<int> edge_indices = {0,1,2,3};

    EdgeDataInt edgeDataInt1 = {0,1,2,0}; // 2 for black
    EdgeDataInt edgeDataInt2 = {1,2,2,0};
    EdgeDataInt edgeDataInt3 = {2,3,2,0};
    EdgeDataInt edgeDataInt4 = {3,0,2,0};
    std::vector<EdgeDataInt> edge_data_int = {
        edgeDataInt1,
        edgeDataInt2,
        edgeDataInt3,
        edgeDataInt4
    };

    EdgeDataFloat edgeDataFloat1 = {1.0f, 1.0f, 1.0f, 0.0f}; // last 0.0f is for padding
    EdgeDataFloat edgeDataFloat2 = {1.0f, 1.0f, 1.0f, 0.0f};
    EdgeDataFloat edgeDataFloat3 = {1.0f, 1.0f, 1.0f, 0.0f};
    EdgeDataFloat edgeDataFloat4 = {1.0f, 1.0f, 1.0f, 0.0f};
    std::vector<EdgeDataFloat> edge_data_float = {
        edgeDataFloat1,
        edgeDataFloat2,
        edgeDataFloat3,
        edgeDataFloat4,
    };

    mesh->Initialize(
        verticies_position,
        verticies_normal,
        verticies_textureuv,
        verticies_materialIndex,
        verticies_textureIndex,
        edge_data_int,
        edge_data_float,
        indices,
        edge_indices
    );
}

std::vector<Position> Mesh::getVerticiesPosition()
{
    return m_verticies_position;
}

std::vector<Normal> Mesh::getVerticiesNormal()
{
    return m_verticies_normal;
}

std::vector<TextureUV> Mesh::getVerticiesTextureUV()
{
    return m_verticies_textureuv;
}

std::vector<int> Mesh::getVerticiesMaterialIndex()
{
    return m_verticies_materialIndex;
}

std::vector<int> Mesh::getVerticiesTextureIndex()
{
    return m_verticies_textureIndex;
}

std::vector<unsigned int> Mesh::getIndices()
{
    return m_indices;
}

std::vector<int> Mesh::getEdgeIndices()
{
    return m_edge_indices;
}

std::vector<EdgeDataInt> Mesh::getEdgeDataInt()
{
    return m_edge_data_int;
}

std::vector<EdgeDataFloat> Mesh::getEdgeDataFloat()
{
    return m_edge_data_float;
}

Position *Mesh::getVerticiesPositionData()
{
    return m_verticies_position.data();
}

Normal *Mesh::getVerticiesNormalData()
{
    return m_verticies_normal.data();
}

TextureUV *Mesh::getVerticiesTextureUVData()
{
    return m_verticies_textureuv.data();
}

int *Mesh::getVerticiesMaterialIndexData()
{
    return m_verticies_materialIndex.data();
}

int *Mesh::getVerticiesTextureIndexData()
{
    return m_verticies_textureIndex.data();
}

float *Mesh::getModelMatriciesData()
{
    return m_model_matrix.data();
}

unsigned int *Mesh::getIndicesData()
{
    return m_indices.data();
}

int *Mesh::getModelMatrixIndicesData()
{
    return m_model_matrix_indices.data();
}

int *Mesh::getEdgeIndicesData()
{
    return m_edge_indices.data();
}

EdgeDataInt *Mesh::getEdgeDataIntData()
{
    return m_edge_data_int.data();
}

EdgeDataFloat *Mesh::getEdgeDataFloatData()
{
    return m_edge_data_float.data();
}

std::array<float, 4> *Mesh::getPickColorData()
{
    return m_pickColor_array.data();
}

unsigned int Mesh::getNumOfVertices()
{
    return m_verticies_position.size();
}

unsigned int Mesh::getNumOfIndices()
{
    return m_indices.size();
}

unsigned int Mesh::getNumOfEdgeIndices()
{
    return m_edge_indices.size();
}

unsigned int Mesh::getNumOfEdges()
{
    return m_edge_indices.size();
}

unsigned int Mesh::getNumOfModelMatricies()
{
    // return m_numOfModelMatrices;
    return m_model_matrix.size();
}

unsigned int Mesh::getNumOfModelMatrixIndices()
{
    return m_model_matrix_indices.size();
}

QMatrix4x4 Mesh::getModelMatrix()
{
    return m_modelMatrix;
}

void Mesh::setModelMatrix(QMatrix4x4 modelMatrix)
{
    m_modelMatrix = modelMatrix;
}

void Mesh::SetModelMatricies(std::vector<QMatrix4x4> &model_matrix)
{
    m_model_matrix.clear();

    m_model_matrix.reserve(model_matrix.size() * 16);

    for (int i = 0; i < model_matrix.size(); ++i) {
        const float* dense = model_matrix[i].constData();  // 16 floats
        m_model_matrix.insert(m_model_matrix.end(), dense, dense + 16);
    }

    m_numOfModelMatrices = model_matrix.size();
}

void Mesh::SetModelMatrixIndices(std::vector<int> &model_matrix_indices)
{
    m_model_matrix_indices = model_matrix_indices;
}

void Mesh::SetPickColorArray(std::vector<std::array<float, 4> > &pickColor_array)
{
    m_pickColor_array = pickColor_array;
}

unsigned int Mesh::getBIMElementId()
{
    return m_bimElementId;
}

void Mesh::setBIMElementId(unsigned int id)
{
    m_bimElementId = id;
}

void Mesh::UpdateGeometry(QVector3D hitPoint)
{
    // m_verticies[0].position[0] = hitPoint.x();
    // m_verticies[0].position[1] = hitPoint.y();
    // m_verticies[0].position[2] = hitPoint.z();
}

void Mesh::encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b)
{
    r = (id & 0x000000FF);
    g = (id & 0x0000FF00) >> 8;
    b = (id & 0x00FF0000) >> 16;
}
