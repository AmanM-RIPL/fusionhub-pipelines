#include "mesh.h"

Mesh::Mesh(QObject *parent)
    : QObject{parent}
{}

void Mesh::Initialize(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    m_verticies = vertices;
    m_indices = indices;
    m_numOfVertices = numOfVertices;
    m_numOfIndices = numOfIndices;
}

GLfloat *Mesh::getVerticies()
{
    return m_verticies;
}

unsigned int *Mesh::getIndices()
{
    return m_indices;
}

unsigned int Mesh::getNumOfVertices()
{
    return m_numOfVertices;
}

unsigned int Mesh::getNumOfIndices()
{
    return m_numOfIndices;
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
