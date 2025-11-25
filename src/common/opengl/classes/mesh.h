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
    static void Combine(Mesh* combinedMesh, QList<Mesh*> meshList);
    static void GenerateBaseSurface(Mesh* mesh);

    std::vector<Vertex> getVerticies();
    std::vector<unsigned int> getIndices();
    std::vector<unsigned int> getBorderIndices();

    Vertex* getVerticiesData();
    QMatrix4x4* getModelMatriciesData();
    unsigned int* getIndicesData();
    unsigned int* getBorderIndicesData();
    int* getModelMatrixIndicesData();


    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    unsigned int getNumOfBorderIndices();
    unsigned int getNumOfModelMatricies();
    unsigned int getNumOfModelMatrixIndices();

    QMatrix4x4 getModelMatrix();

    void SetModelMatricies(std::vector<QMatrix4x4>& model_matrix);
    void SetModelMatrixIndices(std::vector<int>& model_matrix_indices);

    void UpdateGeometry(QVector3D hitPoint);

signals:

private:
    std::vector<Vertex> m_verticies;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_border_indices;
    std::vector<int> m_model_matrix_indices;
    std::vector<QMatrix4x4> m_model_matrix;
    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
    unsigned int m_numOfBorderIndices;
};

#endif // MESH_H
