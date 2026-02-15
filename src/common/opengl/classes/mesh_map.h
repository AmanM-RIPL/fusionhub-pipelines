#ifndef MESH_MAP_H
#define MESH_MAP_H

#include <QObject>

#include "mesh.h"
#include "models/bim_element.h"

#include <map>
#include <vector>

// CombinedMesh is for sending all mesh data in one draw
// call to opengl
struct CombinedMesh
{
    // Vertex Attributes
    std::vector<Position> m_verticies_position;
    std::vector<Normal> m_verticies_normal;
    std::vector<TextureUV> m_verticies_textureuv;
    std::vector<int> m_verticies_materialIndex;
    std::vector<int> m_verticies_textureIndex;

    // Edge Attributes
    std::vector<EdgeDataInt> m_edge_data_int;
    std::vector<EdgeDataFloat> m_edge_data_float;

    std::vector<int> m_model_matrix_indices;
    std::vector<float> m_model_matrix;
    std::vector<std::array<float, 4>> m_pickColor_array;

    // Count
    unsigned int m_numOfVertices;
    unsigned int m_numOfEdges;
    unsigned int m_numOfModelMatrices;

    // Offset
    unsigned int m_verticesOffset;
    unsigned int m_edgesOffset;
    unsigned int m_modelMatricesOffset;
};

struct CombinedIndices
{
    // Indices for IBO
    std::vector<unsigned int> m_indices;
    std::vector<int> m_edge_indices;

    // Count
    unsigned int m_numOfIndices;
    unsigned int m_numOfEdgeIndices;
};

enum ViewType
{
    PLAN,
    MODEL
};

class MeshMap : public QObject
{
    Q_OBJECT
public:
    explicit MeshMap(QObject *parent = nullptr);
    ~MeshMap();

    Mesh* GenerateMesh(); // generates mesh and stores in m_generated_mesh;
    void AddMesh(Mesh* mesh, BIMElement* bim_element, ViewType view_type);
    void SyncDataWithOpenGL(CombinedMesh* combinedMesh);
    void SyncIndicesWithOpenGL(CombinedIndices* combinedIndices, ViewType view_type);
    void MeshMapHasSynced();

signals:


private:
    std::map<int, Mesh*> m_mesh_map_plan; // 2D
    std::map<int, Mesh*> m_mesh_map_model; // 3D
    std::vector<Mesh*> m_mesh_list; // all meshes generated from start of application
    std::set<Mesh*> m_generated_mesh_set;

    // Count till last sync data (becomes offset for the next sync)
    int m_last_sync_index = -1;
    int m_last_sync_numOfVertices = 0;
    int m_last_sync_numOfEdges = 0;
    int m_last_sync_numOfModelMatrices = 0;

    // Current count in m_mesh_list
    int m_numOfVertices = 0;
    int m_numOfEdges = 0;
    int m_numOfModelMatrices = 16; // 4x4 identity matrix is always first
};

#endif // MESH_MAP_H
