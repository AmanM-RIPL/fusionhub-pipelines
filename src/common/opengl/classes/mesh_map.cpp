#include "mesh_map.h"

MeshMap::MeshMap(QObject *parent)
    : QObject{parent}
{}

MeshMap::~MeshMap()
{
    // Delete all generated mesh
    for (Mesh* mesh : m_generated_mesh_set)
    {
        delete mesh;
    }

    m_generated_mesh_set.clear();

    // Delete all mesh in mesh_list
    for (Mesh* mesh : m_mesh_list)
    {
        delete mesh;
    }

    m_mesh_list.clear();

    // Mesh* in m_mesh_list and m_mesh_map_plan/m_mesh_map_model are the same
    // so no need to delete again
}

Mesh *MeshMap::GenerateMesh()
{
    Mesh* mesh = new Mesh();
    m_generated_mesh_set.insert(mesh);

    return mesh;
}

void MeshMap::AddMesh(Mesh *mesh, BIMElement *bim_element, ViewType view_type)
{
    //1. Add to m_mesh_list
    m_mesh_list.push_back(mesh);

    //2. Add to respective map
    int bimElementId = bim_element == nullptr ? 0 : bim_element->getId();

    if (view_type == ViewType::PLAN)
    {
        m_mesh_map_plan[bimElementId] = mesh;
    }
    else
    {
        m_mesh_map_model[bimElementId] = mesh;
    }

    //3. Update mesh's indices and edge_indices based on
    //   global indices in MeshMap
    mesh->OffsetMeshData(m_numOfVertices, m_numOfEdges, m_numOfModelMatrices/16); // m_numOfModelMatrices adds 16 for every matrix

    //4. Update indices
    m_numOfVertices = m_numOfVertices + mesh->getNumOfVertices();
    m_numOfEdges = m_numOfEdges + mesh->getNumOfEdges();

    qInfo() << "viewtype, numVertex, numEdges " << view_type << ", " << m_numOfVertices << ", " << m_numOfEdges;

    QMatrix4x4 modelMatrix = mesh->getModelMatrix();

    if (!modelMatrix.isIdentity())
    {
        m_numOfModelMatrices = m_numOfModelMatrices + 16; // every mesh has only one model matrix of 4x4 floats
    }

    //5. Delete Mesh* from m_generated_mesh_set
    m_generated_mesh_set.erase(mesh);
}

void MeshMap::SyncDataWithOpenGL(CombinedMesh *combinedMesh)
{
    std::vector<Position> verticies_position;
    std::vector<Normal> verticies_normal;
    std::vector<TextureUV> verticies_textureuv;
    std::vector<int> verticies_materialIndex;
    std::vector<int> verticies_textureIndex;

    std::vector<EdgeDataInt> edge_data_int;
    std::vector<EdgeDataFloat> edge_data_float;

    // std::vector<unsigned int> indices;
    // std::vector<int> edge_indices;
    std::vector<std::array<float, 4>> pickColor_array;

    std::vector<int> model_matrix_indices;
    std::vector<QMatrix4x4> model_matrix = {};

    // for the first sync we always send the identity matrix once
    if (m_last_sync_index == -1)
    {
        QMatrix4x4 firstModelMatrix;
        firstModelMatrix.setToIdentity();
        model_matrix.push_back(firstModelMatrix);
    }

    // m_last_sync_index + 1 because we have already sent data till m_last_sync_index
    for (int i = m_last_sync_index + 1; i < m_mesh_list.size(); i++)
    {
        Mesh* mesh = m_mesh_list[i];

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

        QMatrix4x4 meshModelMatrix = mesh->getModelMatrix();

        if (!meshModelMatrix.isIdentity())
        {
            model_matrix.push_back(meshModelMatrix);
        }

        // std::vector<unsigned int> meshIndices = mesh->getIndices();
        // indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

        // std::vector<int> meshEdgeIndices = mesh->getEdgeIndices();
        // edge_indices.insert(edge_indices.end(), meshEdgeIndices.begin(), meshEdgeIndices.end());

        std::vector<EdgeDataInt> meshEdgeDataInt = mesh->getEdgeDataInt();
        edge_data_int.insert(edge_data_int.end(), meshEdgeDataInt.begin(), meshEdgeDataInt.end());

        std::vector<EdgeDataFloat> meshEdgeDataFloat = mesh->getEdgeDataFloat();
        edge_data_float.insert(edge_data_float.end(), meshEdgeDataFloat.begin(),  meshEdgeDataFloat.end());

        std::vector<int> meshModelMatrixIndices = mesh->getModelMatrixIndices();
        model_matrix_indices.insert(model_matrix_indices.end(), meshModelMatrixIndices.begin(), meshModelMatrixIndices.end());

        std::vector<std::array<float, 4>> meshPickColorArray = mesh->getPickColorArray();
        pickColor_array.insert(pickColor_array.end(), meshPickColorArray.begin(), meshPickColorArray.end());
    }

    // Adding data to combined mesh
    combinedMesh->m_verticies_position = verticies_position;
    combinedMesh->m_verticies_normal = verticies_normal;
    combinedMesh->m_verticies_textureuv = verticies_textureuv;
    combinedMesh->m_verticies_materialIndex = verticies_materialIndex;
    combinedMesh->m_verticies_textureIndex = verticies_textureIndex;

    // need to convert std::vector<QMatrix4x4> to std::vector<float> for opengl
    combinedMesh->m_model_matrix.reserve(model_matrix.size() * 16);
    for (int i = 0; i < model_matrix.size(); ++i) {
        const float* dense = model_matrix[i].constData();  // 16 floats
        combinedMesh->m_model_matrix.insert(combinedMesh->m_model_matrix.end(), dense, dense + 16);
    }

    combinedMesh->m_model_matrix_indices = model_matrix_indices;
    combinedMesh->m_pickColor_array = pickColor_array;

    combinedMesh->m_edge_data_int = edge_data_int;
    combinedMesh->m_edge_data_float = edge_data_float;

    combinedMesh->m_numOfVertices = m_numOfVertices - m_last_sync_numOfVertices;
    combinedMesh->m_numOfEdges = m_numOfEdges - m_last_sync_numOfEdges;
    combinedMesh->m_numOfModelMatrices = m_numOfModelMatrices - m_last_sync_numOfModelMatrices;

    combinedMesh->m_verticesOffset = m_last_sync_numOfVertices;
    combinedMesh->m_edgesOffset = m_last_sync_numOfEdges;
    combinedMesh->m_modelMatricesOffset = m_last_sync_numOfModelMatrices;
}

void MeshMap::SyncIndicesWithOpenGL(CombinedIndices *combinedIndices, ViewType view_type)
{
    std::vector<unsigned int> indices;
    std::vector<int> edge_indices;

    auto meshMap = view_type == ViewType::PLAN ? m_mesh_map_plan : m_mesh_map_model;

    qInfo() << "View Type: --------------------------------" << view_type;

    for (auto& meshMapItem: meshMap)
    {
        Mesh* mesh = meshMapItem.second;
        std::vector<unsigned int> meshIndices = mesh->getIndices();
        indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

        std::vector<int> meshEdgeIndices = mesh->getEdgeIndices();
        edge_indices.insert(edge_indices.end(), meshEdgeIndices.begin(), meshEdgeIndices.end());

        for (int& mesh_ind: meshEdgeIndices)
        {
            qInfo() << "Edge Index: " << mesh_ind;
        }
    }

    // Adding data to combined mesh
    combinedIndices->m_indices = indices;
    combinedIndices->m_edge_indices = edge_indices;
    combinedIndices->m_numOfIndices = indices.size();
    combinedIndices->m_numOfEdgeIndices = edge_indices.size();
}

void MeshMap::MeshMapHasSynced()
{
    m_last_sync_index = m_mesh_list.size() - 1; // m_last_sync_index = -1 means no sync has happened

    m_last_sync_numOfVertices = m_numOfVertices;
    m_last_sync_numOfEdges = m_numOfEdges;
    m_last_sync_numOfModelMatrices = m_numOfModelMatrices;
}
