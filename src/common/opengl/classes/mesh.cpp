#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject{parent}
{}

void Mesh::Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& borderIndices, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfBorderIndices)
{
    m_verticies = vertices;
    m_indices = indices;
    m_border_indices = borderIndices;
    m_numOfVertices = numOfVertices;
    m_numOfIndices = numOfIndices;
    m_numOfBorderIndices = numOfBorderIndices;
}

void Mesh::Copy(Mesh *mesh)
{
     mesh->Initialize(m_verticies, m_indices, m_border_indices, m_numOfVertices, m_numOfIndices, m_numOfBorderIndices);
}

void Mesh::Combine(Mesh *combinedMesh, QList<Mesh *> meshList)
{
    std::vector<Vertex> verticies;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> border_indices;

    std::vector<int> model_matrix_indices;
    std::vector<QMatrix4x4> model_matrix = {};
    QMatrix4x4 firstModelMatrix;
    firstModelMatrix.setToIdentity();
    model_matrix.push_back(firstModelMatrix);

    unsigned int numOfVertices = 0;
    unsigned int numOfIndices = 0;
    unsigned int numOfBorderIndices = 0;

    for (Mesh* mesh: meshList)
    {
        std::vector<Vertex> meshVerticies = mesh->getVerticies();
        verticies.insert(verticies.end(), meshVerticies.begin(),  meshVerticies.end());

        unsigned int modelIndex = 0; // 0 for identity matrix
        QMatrix4x4 meshModelMatrix = mesh->getModelMatrix();

        if (!meshModelMatrix.isIdentity())
        {
            model_matrix.push_back(meshModelMatrix);
            modelIndex = model_matrix.size();
        }

        for (Vertex meshVertex: meshVerticies)
        {
            model_matrix_indices.push_back(modelIndex);
        }

        std::vector<unsigned int> meshIndices = mesh->getIndices();
        for (unsigned int& index: meshIndices)
        {
            index += numOfVertices;
        }
        indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

        std::vector<unsigned int> meshBorderIndices = mesh->getBorderIndices();
        for (unsigned int& index: meshBorderIndices)
        {
            index += numOfVertices;
        }
        border_indices.insert(border_indices.end(), meshBorderIndices.begin(), meshBorderIndices.end());

        numOfVertices += mesh->getNumOfVertices();
        numOfIndices += mesh->getNumOfIndices();
        numOfBorderIndices += mesh->getNumOfBorderIndices();
    }


    combinedMesh->Initialize(verticies, indices, border_indices, numOfVertices, numOfIndices, numOfBorderIndices);
    combinedMesh->SetModelMatricies(model_matrix);
    combinedMesh->SetModelMatrixIndices(model_matrix_indices);
}

void Mesh::GenerateBaseSurface(Mesh *mesh)
{
    std::vector<Vertex> verticies = {
        {
            {-10.0f, -10.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            {0.0f, 0.0f}, // texture uv
            1, // materialIndex
            -1 // textureIndex
        },
        {
            {-10.0f, 10.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            {0.0f, 0.0f}, // texture uv
            1, // materialIndex
            -1 // textureIndex
        },
        {
            {10.0f, 10.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            {0.0f, 0.0f}, // texture uv
            1, // materialIndex
            -1 // textureIndex
        },
        {
            {10.0f, -10.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            {0.0f, 0.0f}, // texture uv
            1, // materialIndex
            -1 // textureIndex
        }
    };
    std::vector<unsigned int> indices = {0,2,1, 0,3,2}; // winding order as face culling is switched on
    std::vector<unsigned int> border_indices = {0,1,1,2,2,3,3,0};
    unsigned int numOfVertices = 4;
    unsigned int numOfIndices = 6;
    unsigned int numOfBorderIndices = 8;

    mesh->Initialize(verticies, indices, border_indices, numOfVertices, numOfIndices, numOfBorderIndices);
}

std::vector<Vertex> Mesh::getVerticies()
{
    return m_verticies;
}

std::vector<unsigned int> Mesh::getIndices()
{
    return m_indices;
}

std::vector<unsigned int> Mesh::getBorderIndices()
{
    return m_border_indices;
}

Vertex *Mesh::getVerticiesData()
{
    return m_verticies.data();
}

QMatrix4x4 *Mesh::getModelMatriciesData()
{
    return m_model_matrix.data();
}

unsigned int *Mesh::getIndicesData()
{
    return m_indices.data();
}

unsigned int *Mesh::getBorderIndicesData()
{
    return m_border_indices.data();
}

int *Mesh::getModelMatrixIndicesData()
{
    return m_model_matrix_indices.data();
}

unsigned int Mesh::getNumOfVertices()
{
    return m_numOfVertices;
}

unsigned int Mesh::getNumOfIndices()
{
    return m_numOfIndices;
}

unsigned int Mesh::getNumOfBorderIndices()
{
    return m_numOfBorderIndices;
}

unsigned int Mesh::getNumOfModelMatricies()
{
    return m_model_matrix.size();
}

unsigned int Mesh::getNumOfModelMatrixIndices()
{
    return m_model_matrix_indices.size();
}

QMatrix4x4 Mesh::getModelMatrix()
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();

    return matrix;
}

void Mesh::SetModelMatricies(std::vector<QMatrix4x4> &model_matrix)
{
    m_model_matrix = model_matrix;
}

void Mesh::SetModelMatrixIndices(std::vector<int> &model_matrix_indices)
{
    m_model_matrix_indices = model_matrix_indices;
}

void Mesh::UpdateGeometry(QVector3D hitPoint)
{
    m_verticies[0].position[0] = hitPoint.x();
    m_verticies[0].position[1] = hitPoint.y();
    m_verticies[0].position[2] = hitPoint.z();
}
