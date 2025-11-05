#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>

#include <vector>

struct Vertex
{
    float position[3];
    float normal[3];
    float uv[2];
    int materialIndex;
    int textureIndex;
};

class Mesh : public QObject
{
    Q_OBJECT
public:
    explicit Mesh(QObject *parent = nullptr);

    void Initialize(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& borderIndices, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfBorderIndices);
    void Copy(Mesh* mesh);

    Vertex* getVerticies();
    unsigned int* getIndices();
    unsigned int* getBorderIndices();
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    unsigned int getNumOfBorderIndices();
    QMatrix4x4 getModelMatrix();

    void UpdateGeometry(QVector3D hitPoint);

signals:

private:
    std::vector<Vertex> m_verticies;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_border_indices;
    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
    unsigned int m_numOfBorderIndices;
};

#endif // MESH_H
