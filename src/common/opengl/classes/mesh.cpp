#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject{parent}
{}

void Mesh::Initialize(const std::vector<GLfloat>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& borderIndices, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfBorderIndices)
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

GLfloat *Mesh::getVerticies()
{
    return m_verticies.data();
}

unsigned int *Mesh::getIndices()
{
    return m_indices.data();
}

unsigned int *Mesh::getBorderIndices()
{
    return m_border_indices.data();
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

QMatrix4x4 Mesh::getModelMatrix()
{
    QMatrix4x4 matrix;
    matrix.setToIdentity();

    return matrix;
}

void Mesh::UpdateGeometry(QVector3D hitPoint)
{
    m_verticies[0] = hitPoint.x();
    m_verticies[1] = hitPoint.y();
    m_verticies[2] = hitPoint.z();
}
