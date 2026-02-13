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
    if (view_type == ViewType::PLAN)
    {
        m_mesh_map_plan[bim_element->getId()] = mesh;
    }
    else
    {
        m_mesh_map_model[bim_element->getId()] = mesh;
    }

    //3. Update mesh's indices and edge_indices based on
    //   global indices in MeshMap
    mesh->OffsetVerticesAndEdges(m_numOfVertices, m_numOfEdges);

    //4. Update indices
    m_numOfVertices = m_numOfVertices + mesh->getNumOfVertices();
    m_numOfIndices = m_numOfIndices + mesh->getNumOfIndices();
    m_numOfEdges = m_numOfEdges + mesh->getNumOfEdges();
    m_numOfEdgeIndices = m_numOfEdgeIndices + mesh->getNumOfEdgeIndices();
    m_numOfModelMatrices = m_numOfModelMatrices + mesh->getNumOfModelMatricies();

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
}
