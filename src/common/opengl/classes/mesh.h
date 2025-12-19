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
    void AppendGeometry(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& borderIndices);
    void Copy(Mesh* mesh);

    static void Combine(Mesh* combinedMesh, QList<Mesh*> meshList);
    static void GenerateBaseSurface(Mesh* mesh);

    std::vector<Vertex> getVerticies();
    std::vector<unsigned int> getIndices();
    std::vector<unsigned int> getBorderIndices();

    Vertex* getVerticiesData();
    float* getModelMatriciesData();
    unsigned int* getIndicesData();
    unsigned int* getBorderIndicesData();
    int* getModelMatrixIndicesData();
    std::array<float, 4>* getPickColorData();


    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    unsigned int getNumOfBorderIndices();
    unsigned int getNumOfModelMatricies();
    unsigned int getNumOfModelMatrixIndices();

    QMatrix4x4 getModelMatrix();
    void setModelMatrix(QMatrix4x4 modelMatrix);

    void SetModelMatricies(std::vector<QMatrix4x4>& model_matrix);
    void SetModelMatrixIndices(std::vector<int>& model_matrix_indices);
    void SetPickColorArray(std::vector<std::array<float, 4>>& pickColor_array);

    unsigned int getBIMElementId();
    void setBIMElementId(unsigned int id);

    void UpdateGeometry(QVector3D hitPoint);

signals:

private:
    std::vector<Vertex> m_verticies;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_border_indices;
    std::vector<int> m_model_matrix_indices;

    std::vector<float> m_model_matrix; // this is for the combined mesh
    std::vector<std::array<float, 4>> m_pickColor_array; // this is for the combined mesh

    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
    unsigned int m_numOfBorderIndices;
    unsigned int m_numOfModelMatrices;


    unsigned int m_bimElementId = 1; // default value is 1
    QMatrix4x4 m_modelMatrix; // this is for the individual mesh

    static void encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b);
};

#endif // MESH_H
