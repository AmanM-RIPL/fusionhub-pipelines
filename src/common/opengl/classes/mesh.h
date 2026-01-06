#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>

#include <vector>

// struct Vertex
// {
//     float position[3];
//     float normal[3];
//     float uv[2];
//     int materialIndex;
//     int textureIndex;
// };

using Position = std::array<float, 4>;
using Normal = std::array<float, 3>;
using TextureUV = std::array<float, 2>;
using EdgeIndex = std::array<int, 2>; // EdgeIndex is of the format {0, 1} which means edge b/w 0th Position and 1st Position

class Mesh : public QObject
{
    Q_OBJECT
public:
    explicit Mesh(QObject *parent = nullptr);

    void Initialize(
        const std::vector<Position>& vertices_position,
        const std::vector<Normal>& vertices_normal,
        const std::vector<TextureUV>& vertices_textureuv,
        const std::vector<int>& vertices_materialIndex,
        const std::vector<int>& vertices_textureIndex,

        const std::vector<EdgeIndex>& edge_indices,
        const std::vector<float>& edge_width,
        const std::vector<float>& edge_dashLength,
        const std::vector<float>& edge_gapLength,
        const std::vector<int>& edge_dash,
        const std::vector<int>& edge_materialIndex,

        const std::vector<unsigned int>& indices
    );

    void AppendGeometry(
        const std::vector<Position>& vertices_position,
        const std::vector<Normal>& vertices_normal,
        const std::vector<TextureUV>& vertices_textureuv,
        const std::vector<int>& vertices_materialIndex,
        const std::vector<int>& vertices_textureIndex,

        const std::vector<EdgeIndex>& edge_indices,
        const std::vector<float>& edge_width,
        const std::vector<float>& edge_dashLength,
        const std::vector<float>& edge_gapLength,
        const std::vector<int>& edge_dash,
        const std::vector<int>& edge_materialIndex,

        const std::vector<unsigned int>& indices
    );

    void Copy(Mesh* mesh);

    static void Combine(Mesh* combinedMesh, QList<Mesh*> meshList);
    static void GenerateBaseSurface(Mesh* mesh);

    std::vector<Position> getVerticiesPosition();
    std::vector<Normal> getVerticiesNormal();
    std::vector<TextureUV> getVerticiesTextureUV();
    std::vector<int> getVerticiesMaterialIndex();
    std::vector<int> getVerticiesTextureIndex();

    std::vector<EdgeIndex> getEdgeIndices();
    std::vector<float> getEdgeWidth();
    std::vector<float> getEdgeDashLength();
    std::vector<float> getEdgeGapLength();
    std::vector<int> getEdgeDash();
    std::vector<int> getEdgeMaterialIndex();

    std::vector<unsigned int> getIndices();


    Position* getVerticiesPositionData();
    Normal* getVerticiesNormalData();
    TextureUV* getVerticiesTextureUVData();
    int* getVerticiesMaterialIndexData();
    int* getVerticiesTextureIndexData();

    EdgeIndex* getEdgeIndicesData();
    float* getEdgeWidthData();
    float* getEdgeDashLengthData();
    float* getEdgeGapLengthData();
    int* getEdgeDashData();
    int* getEdgeMaterialIndexData();

    float* getModelMatriciesData();
    unsigned int* getIndicesData();
    int* getModelMatrixIndicesData();
    std::array<float, 4>* getPickColorData();


    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    unsigned int getNumOfEdges();
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

    // Vertex Attributes
    std::vector<Position> m_verticies_position;
    std::vector<Normal> m_verticies_normal;
    std::vector<TextureUV> m_verticies_textureuv;
    std::vector<int> m_verticies_materialIndex;
    std::vector<int> m_verticies_textureIndex;

    // Edge Attributes
    std::vector<EdgeIndex> m_edge_indices;
    std::vector<float> m_edge_width;
    std::vector<float> m_edge_dashLength;
    std::vector<float> m_edge_gapLength;
    std::vector<int> m_edge_dash; // to show dashes or not
    std::vector<int> m_edge_materialIndex;

    // Indices for IBO
    std::vector<unsigned int> m_indices;
    std::vector<int> m_model_matrix_indices;

    std::vector<float> m_model_matrix; // this is for the combined mesh
    std::vector<std::array<float, 4>> m_pickColor_array; // this is for the combined mesh

    unsigned int m_numOfVertices;
    unsigned int m_numOfEdges;
    unsigned int m_numOfIndices;
    unsigned int m_numOfModelMatrices;


    unsigned int m_bimElementId = 1; // default value is 1
    QMatrix4x4 m_modelMatrix; // this is for the individual mesh

    static void encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b);
};

#endif // MESH_H
