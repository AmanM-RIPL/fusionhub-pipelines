#include "view.h"

View::View(QObject *parent)
    : QObject{parent}
{
}

void View::Initialize()
{
    this->initializeOpenGLFunctions();

    this->glEnable(GL_DEPTH_TEST);

    InitializeHandles();
    InitializeStaticBuffers();
    InitializeDynamicBuffers();
}

void View::LoadStaticMeshData(QList<Mesh*>& meshList)
{
    delete combinedMesh;
    combinedMesh = new Mesh();
    Mesh::Combine(combinedMesh, meshList);

    Position* vertices_position = combinedMesh->getVerticiesPositionData();
    Normal* vertices_normal = combinedMesh->getVerticiesNormalData();
    TextureUV* vertices_textureuv = combinedMesh->getVerticiesTextureUVData();
    int* vertices_materialIndex = combinedMesh->getVerticiesMaterialIndexData();
    int* vertices_textureIndex = combinedMesh->getVerticiesTextureIndexData();

    std::array<float, 4> corners = {-1.0f, 1.0f, -1.0f, 1.0f};
    EdgeIndex* edge_indices = combinedMesh->getEdgeIndicesData();
    float* edge_width = combinedMesh->getEdgeWidthData();
    float* edge_dashLength = combinedMesh->getEdgeDashLengthData();
    float* edge_gapLength = combinedMesh->getEdgeGapLengthData();
    int* edge_dash = combinedMesh->getEdgeDashData();
    int* edge_materialIndex = combinedMesh->getEdgeMaterialIndexData();

    float* modelMatrices = combinedMesh->getModelMatriciesData();
    unsigned int* indices = combinedMesh->getIndicesData();

    int* modelMatrixIndices = combinedMesh->getModelMatrixIndicesData();
    std::array<float, 4>* pickColors = combinedMesh->getPickColorData();

    unsigned int numOfVertices = combinedMesh->getNumOfVertices();
    unsigned int numOfIndices = combinedMesh->getNumOfIndices();
    unsigned int numOfEdges = combinedMesh->getNumOfEdges();
    unsigned int numOfModelMatrices = combinedMesh->getNumOfModelMatricies();
    unsigned int numOfModelMatrixIndices = combinedMesh->getNumOfModelMatrixIndices();

    // for (int i = 0; i < numOfModelMatrixIndices; i++)
    // {
    //     qInfo() << "Model Matrix Index: " << modelMatrixIndices[i];
    // }

    // Initializing the vao, vbo, and ibo
    m_static_indexCount = numOfIndices;
    m_static_borderIndexCount = numOfEdges;

    int numOfVerticesDefault = 100000;
    int numOfIndicesDefault = 100000;
    int numOfEdgesDefault = 100000;
    int numOfModelMatricesDefault = 1000;
    int numOfModelMatrixIndicesDefault = 100000;

    // for (int i = 0; i < numOfVertices; i++)
    // {
    //     Position pos = vertices_position[i];
    //     qInfo() << "Position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")";
    // }

    // for (int i = 0; i < numOfEdges; i++)
    // {
    //     EdgeIndex edge = edge_indices[i];
    //     qInfo() << "Edge: (" << edge[0] << ", " << edge[1] << ")";
    // }

    // VAO for Mesh + Mesh Color Picking
    this->glBindVertexArray(m_vao);
        // Buffer Orphaning
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_position_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_normal_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureuv_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(TextureUV) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_materialIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_model_matrix_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfModelMatrixIndicesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_pick_color_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(std::array<float, 4>) * numOfVerticesDefault, NULL, GL_STATIC_DRAW);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
        this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfIndicesDefault, NULL, GL_STATIC_DRAW);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_static_matrix_tbo);
        this->glBufferData(GL_TEXTURE_BUFFER, numOfModelMatricesDefault * sizeof(float), NULL, GL_STATIC_DRAW);



        // Reloading Data
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_position_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Position) * numOfVertices, vertices_position);


        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_normal_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Normal) * numOfVertices, vertices_normal);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureuv_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureUV) * numOfVertices, vertices_textureuv);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_materialIndex_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfVertices, vertices_materialIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureIndex_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfVertices, vertices_textureIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_model_matrix_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfModelMatrixIndices, modelMatrixIndices);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_pick_color_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(std::array<float, 4>) * numOfVertices, pickColors);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * numOfIndices, indices);

        // //LINE IBO
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_border_ibo);
        //     this->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfBorderIndices, borderIndices, GL_STATIC_DRAW);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_static_matrix_tbo);
            this->glBufferSubData(GL_TEXTURE_BUFFER, 0, numOfModelMatrices * sizeof(float), modelMatrices);

    this->glBindVertexArray(0);

    // VAO for edge
    this->glBindVertexArray(m_edge_vao);
        // Buffer Orphaning
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_indices_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(EdgeIndex) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_width_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_materialIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dashLength_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_gapLength_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dash_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STATIC_DRAW);


        // Reloading Data
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_indices_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EdgeIndex) * numOfEdges, edge_indices);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_width_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_width);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_materialIndex_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfEdges, edge_materialIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dashLength_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_dashLength);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_gapLength_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_gapLength);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dash_vbo);
            this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfEdges, edge_dash);
    this->glBindVertexArray(0);
}

void View::LoadDynamicMeshData(Mesh* mesh)
{
    int numOfVerticesDefault = 100000;
    int numOfIndicesDefault = 100000;
    int numOfEdgesDefault = 100000;
    int numOfModelMatricesDefault = 1000;
    int numOfModelMatrixIndicesDefault = 100000;

    if (mesh == nullptr)
    {
        // VAO for Mesh + Mesh Color Picking
        this->glBindVertexArray(m_dynamic_vao);
            // Buffer Orphaning
            //VBO
            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_position_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);


            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_normal_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureuv_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(TextureUV) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_model_matrix_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfModelMatrixIndicesDefault, NULL, GL_STREAM_DRAW);

            //TRIANGLE IBO
            this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dynamic_ibo);
            this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfIndicesDefault, NULL, GL_STREAM_DRAW);

            // TBO for Model Matrix
            this->glBindBuffer(GL_TEXTURE_BUFFER, m_dynamic_matrix_tbo);
            this->glBufferData(GL_TEXTURE_BUFFER, numOfModelMatricesDefault * sizeof(float), NULL, GL_STREAM_DRAW);
        this->glBindVertexArray(0);

        // VAO for edge
        this->glBindVertexArray(m_dynamic_edge_vao);
            // Buffer Orphaning
            //VBO
            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_indices_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(EdgeIndex) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_width_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dashLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_gapLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

            this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dash_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);
        this->glBindVertexArray(0);

        return;
    }




    Position* vertices_position = mesh->getVerticiesPositionData();
    Normal* vertices_normal = mesh->getVerticiesNormalData();
    TextureUV* vertices_textureuv = mesh->getVerticiesTextureUVData();
    int* vertices_materialIndex = mesh->getVerticiesMaterialIndexData();
    int* vertices_textureIndex = mesh->getVerticiesTextureIndexData();

    EdgeIndex* edge_indices = mesh->getEdgeIndicesData();
    float* edge_width = mesh->getEdgeWidthData();
    float* edge_dashLength = mesh->getEdgeDashLengthData();
    float* edge_gapLength = mesh->getEdgeGapLengthData();
    int* edge_dash = mesh->getEdgeDashData();
    int* edge_materialIndex = mesh->getEdgeMaterialIndexData();

    float* modelMatrices = mesh->getModelMatriciesData();
    unsigned int* indices = mesh->getIndicesData();

    int* modelMatrixIndices = mesh->getModelMatrixIndicesData();

    unsigned int numOfVertices = mesh->getNumOfVertices();
    unsigned int numOfIndices = mesh->getNumOfIndices();
    unsigned int numOfEdges = mesh->getNumOfEdges();
    unsigned int numOfModelMatrices = mesh->getNumOfModelMatricies();
    unsigned int numOfModelMatrixIndices = mesh->getNumOfModelMatrixIndices();

    // Initializing the vao, vbo, and ibo
    m_dynamic_indexCount = numOfIndices;
    m_dynamic_borderIndexCount = numOfEdges;

    // for (int i = 0; i < numOfVertices; i++)
    // {
    //     Position pos = vertices_position[i];
    //     qInfo() << "Position: (" << pos[0] << ", " << pos[1] << ", " << pos[2] << ")";
    // }

    // for (int i = 0; i < numOfEdges; i++)
    // {
    //     EdgeIndex edge = edge_indices[i];
    //     qInfo() << "Edge: (" << edge[0] << ", " << edge[1] << ")";
    // }

    // VAO for Mesh + Mesh Color Picking
    this->glBindVertexArray(m_dynamic_vao);
        // Buffer Orphaning
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_position_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);


        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_normal_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureuv_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(TextureUV) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_materialIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVerticesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_model_matrix_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfModelMatrixIndicesDefault, NULL, GL_STREAM_DRAW);

        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dynamic_ibo);
        this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfIndicesDefault, NULL, GL_STREAM_DRAW);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_dynamic_matrix_tbo);
        this->glBufferData(GL_TEXTURE_BUFFER, numOfModelMatricesDefault * sizeof(float), NULL, GL_STREAM_DRAW);



        // Reloading data
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_position_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Position) * numOfVertices, vertices_position);


        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_normal_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Normal) * numOfVertices, vertices_normal);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureuv_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TextureUV) * numOfVertices, vertices_textureuv);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_materialIndex_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfVertices, vertices_materialIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureIndex_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfVertices, vertices_textureIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_model_matrix_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfModelMatrixIndices, modelMatrixIndices);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dynamic_ibo);
        this->glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(unsigned int) * numOfIndices, indices);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_dynamic_matrix_tbo);
        this->glBufferSubData(GL_TEXTURE_BUFFER, 0, numOfModelMatrices * sizeof(float), modelMatrices);

    this->glBindVertexArray(0);

    // VAO for edge
    this->glBindVertexArray(m_dynamic_edge_vao);
        // Buffer Orphaning
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_indices_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(EdgeIndex) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_width_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_materialIndex_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dashLength_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_gapLength_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dash_vbo);
        this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdgesDefault, NULL, GL_STREAM_DRAW);

        // Reloading data
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_indices_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(EdgeIndex) * numOfEdges, edge_indices);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_width_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_width);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_materialIndex_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfEdges, edge_materialIndex);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dashLength_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_dashLength);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_gapLength_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * numOfEdges, edge_gapLength);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dash_vbo);
        this->glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(int) * numOfEdges, edge_dash);
    this->glBindVertexArray(0);
}


void View::Render()
{
    this->glEnable(GL_DEPTH_TEST);
    this->glEnable(GL_MULTISAMPLE);
    this->glDepthFunc(GL_LEQUAL);
    this->glDepthMask(GL_TRUE);
    this->glDisable(GL_BLEND);

    this->glEnable(GL_CULL_FACE);
    this->glCullFace(GL_BACK);
    this->glFrontFace(GL_CW); // GL_CW or GL_CCW ??? GL_CW seems to work because we flipped Y in the vertex shader

    this->glViewport(0, 0, viewportWidth, viewportHeight);
    this->glClearColor(0.97f, 0.99f, 0.98f, 1.0f);
    this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene with the required colors
    this->glUseProgram(shader->getShaderId());

    QVector3D cameraPosition = camera->getCameraPosition();
    this->glUniform3f(shader->getViewPositionId(), cameraPosition.x(), cameraPosition.y(), cameraPosition.z());

    // For texture arrays
    this->glUniform1i(shader->getTextureArrayId(), 0);

    // For Model Matrices
    this->glUniform1i(shader->getModelMatrixBufferId(), 2);

    // For Material List
    this->glUniform1i(shader->getMaterialBufferId(), 1);

    this->glUniform3f(shader->getLightPositionId(), 20.0f, 0.0f, 0.0f);
    this->glUniform3f(shader->getLightAmbientId(), 1.0f, 1.0f, 1.0f); // 0.2f, 0.2f, 0.2f
    this->glUniform3f(shader->getLightDiffuseId(), 0.5f, 0.5f, 0.5f); // 0.5f, 0.5f, 0.5f
    this->glUniform3f(shader->getLightSpecularId(), 1.0f, 1.0f, 1.0f);

    // --- Upload to shader ---
    this->glUniformMatrix4fv(shader->getViewId(),  1, GL_FALSE, camera->calculateViewMatrix().constData());
    this->glUniformMatrix4fv(shader->getProjectionId(),  1, GL_FALSE, m_projectionMatrix.constData());



    // Draw
    this->glBindVertexArray(m_vao);
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glDrawElements(GL_TRIANGLES, m_static_indexCount, GL_UNSIGNED_INT, 0);
    this->glBindVertexArray(0);

    // For Model Matrices
    this->glUniform1i(shader->getModelMatrixBufferId(), 5);

    this->glBindVertexArray(m_dynamic_vao);
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dynamic_ibo);
            this->glDrawElements(GL_TRIANGLES, m_dynamic_indexCount, GL_UNSIGNED_INT, 0);
    this->glBindVertexArray(0);




    // Edge Shader Program
    this->glDisable(GL_CULL_FACE);

    this->glUseProgram(edgeShader->getShaderId());

    this->glUniformMatrix4fv(edgeShader->getViewId(),  1, GL_FALSE, camera->calculateViewMatrix().constData());
    this->glUniformMatrix4fv(edgeShader->getProjectionId(),  1, GL_FALSE, m_projectionMatrix.constData());

    std::array<float, 2> viewportArray = {viewportWidth, viewportHeight};
    this->glUniform2fv(edgeShader->getViewPortId(), 1, viewportArray.data());

    // For vertices arrays
    this->glUniform1i(edgeShader->getVerticesId(), 3);

    // For Model Matrices
    this->glUniform1i(edgeShader->getModelMatrixBufferId(), 2);

    // For Model Matrix Indices
    this->glUniform1i(edgeShader->getModelMatrixIndexBufferId(), 4);

    // For Material List
    this->glUniform1i(edgeShader->getMaterialBufferId(), 1);

    this->glUniform3f(edgeShader->getLightPositionId(), 20.0f, 0.0f, 0.0f);
    this->glUniform3f(edgeShader->getLightAmbientId(), 1.0f, 1.0f, 1.0f); // 0.2f, 0.2f, 0.2f
    this->glUniform3f(edgeShader->getLightDiffuseId(), 0.5f, 0.5f, 0.5f); // 0.5f, 0.5f, 0.5f
    this->glUniform3f(edgeShader->getLightSpecularId(), 1.0f, 1.0f, 1.0f);

    // Draw
    this->glBindVertexArray(m_edge_vao);
        this->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_static_borderIndexCount);
    this->glBindVertexArray(0);


    // For vertices arrays
    this->glUniform1i(edgeShader->getVerticesId(), 6);

    // For Model Matrices
    this->glUniform1i(edgeShader->getModelMatrixBufferId(), 5);

    // For Model Matrix Indices
    this->glUniform1i(edgeShader->getModelMatrixIndexBufferId(), 7);

    this->glBindVertexArray(m_dynamic_edge_vao);
        this->glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, m_dynamic_borderIndexCount);
    this->glBindVertexArray(0);
}

unsigned int View::Selection()
{
    // ensure pick FBO exists and matches size
    ensurePickFBO();

    if (m_pickFBO)
    {
        // Bind picking framebuffer
        this->glBindFramebuffer(GL_FRAMEBUFFER, m_pickFBO);
            this->glEnable(GL_DEPTH_TEST);
            this->glDepthFunc(GL_LEQUAL);
            this->glDepthMask(GL_TRUE);
            this->glDisable(GL_BLEND);

            this->glEnable(GL_CULL_FACE);
            this->glCullFace(GL_BACK);
            this->glFrontFace(GL_CW); // GL_CW or GL_CCW ??? GL_CW seems to work because we flipped Y in the vertex shader

            this->glViewport(0, 0, viewportWidth, viewportHeight);
            this->glClearColor(0,0,0,0);
            this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // draw scene with flat pick colors
            this->glUseProgram(pickingShader->getShaderId());

            unsigned int objectId = 1;
            unsigned char r,g,b;
            encodeIdToColor(objectId, r,g,b);
            this->glUniform4f(pickingShader->getPickColorId(), r/255.0f, g/255.0f, b/255.0f, 1.0f);

            // --- Upload to shader ---
            this->glUniform1i(pickingShader->getModelMatrixBufferId(), 2);

            this->glUniformMatrix4fv(pickingShader->getViewId(),  1, GL_FALSE, camera->calculateViewMatrix().constData());
            this->glUniformMatrix4fv(pickingShader->getProjectionId(),  1, GL_FALSE, m_projectionMatrix.constData());

            this->glBindVertexArray(m_vao);
                this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
                    this->glDrawElements(GL_TRIANGLES, m_static_indexCount, GL_UNSIGNED_INT, 0);
                // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            this->glBindVertexArray(0);

            // Read pixel
            int readX = m_pickX;
            int readY = m_pickY; //(viewportHeight - 1) - m_pickY;
            unsigned char pixel[4];
            this->glReadPixels(readX, readY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

            unsigned int pickedId = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);

        // Restore default framebuffer
        this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

        // qInfo() << "Picked Id: " << pickedId;
        return pickedId;
    }
}

QVector3D View::GetPointInModelSpace(Mesh* mesh)
{
    // --- Step 1: Screen -> NDC
    float x = (2.0f * m_pickX) / float(viewportWidth) - 1.0f;
    float y = 1.0f - (2.0f * m_pickY) / float(viewportHeight);
    QVector4D rayClip(x, y, -1.0f, 1.0f);

    // --- Step 2: NDC -> Eye space
    QVector4D rayEye = m_projectionMatrix.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(), rayEye.y(), -1.0f, 0.0f);

    // --- Step 3: Eye -> World space
    QVector3D rayDirWorld = (camera->calculateViewMatrix().inverted() * rayEye).toVector3D().normalized();
    QVector3D rayOriginWorld = camera->getCameraPosition();

    // --- Step 4: Transform ray into *model space*
    QMatrix4x4 invModel = mesh->getModelMatrix().inverted();
    QVector3D rayOriginModel = (invModel * QVector4D(rayOriginWorld, 1.0f)).toVector3D();
    QVector3D rayDirModel    = (invModel * QVector4D(rayDirWorld, 0.0f)).toVector3D().normalized();

    // --- Step 5: Ray-plane intersection in model space (Z=0 plane)
    QVector3D planeNormal(0, 0, 1);
    QVector3D planePoint(0, 0, 0);
    float denom = QVector3D::dotProduct(planeNormal, rayDirModel);
    // if (fabs(denom) < 1e-6f) {
    //     qWarning() << "Ray parallel to model plane, no intersection";
    //     return;
    // }
    float t = QVector3D::dotProduct(planePoint - rayOriginModel, planeNormal) / denom;
    // if (t < 0) {
    //     qWarning() << "Intersection is behind camera";
    //     return;
    // }

    QVector3D hitPoint = rayOriginModel + t * rayDirModel;

    return hitPoint;
}

QVector3D View::GetPointInViewSpace(int pointX, int pointY)
{
    // --- Step 1: Screen -> NDC
    float x = (2.0f * pointX) / float(viewportWidth) - 1.0f;
    float y = 1.0f - (2.0f * pointY) / float(viewportHeight);
    QVector4D rayClip(x, y, -1.0f, 1.0f);

    // --- Step 2: NDC -> Eye space
    QVector4D rayEye = m_projectionMatrix.inverted() * rayClip;
    rayEye = QVector4D(rayEye.x(), rayEye.y(), -1.0f, 0.0f);

    // --- Step 3: Eye -> World space
    QVector3D rayDirWorld = (camera->calculateViewMatrix().inverted() * rayEye).toVector3D().normalized();
    QVector3D rayOriginWorld = camera->getCameraPosition();

    // --- Step 4: Transform ray into *model space*
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    QMatrix4x4 invModel = matrix.inverted();
    QVector3D rayOriginModel = (invModel * QVector4D(rayOriginWorld, 1.0f)).toVector3D();
    QVector3D rayDirModel    = (invModel * QVector4D(rayDirWorld, 0.0f)).toVector3D().normalized();

    // --- Step 5: Ray-plane intersection in model space (Z=0 plane)
    QVector3D planeNormal(0, 0, 1);
    QVector3D planePoint(0, 0, 0);
    float denom = QVector3D::dotProduct(planeNormal, rayDirModel);
    // if (fabs(denom) < 1e-6f) {
    //     qWarning() << "Ray parallel to model plane, no intersection";
    //     return;
    // }
    float t = QVector3D::dotProduct(planePoint - rayOriginModel, planeNormal) / denom;
    // if (t < 0) {
    //     qWarning() << "Intersection is behind camera";
    //     return;
    // }

    QVector3D hitPoint = rayOriginModel + t * rayDirModel;

    return hitPoint;
}

std::array<float, 2> View::GetPointInScreenSpace(QVector3D& point3D)
{
    QRect viewport(0, 0, viewportWidth, viewportHeight);

    QVector3D screenPos = point3D.project(camera->calculateViewMatrix(), m_projectionMatrix, viewport);

    return { screenPos.x(),  (viewportHeight - screenPos.y()) };
}

void View::AddMesh(Mesh *mesh)
{
    // meshList.append(mesh);
}

void View::DeleteAllMesh()
{
    // Only clears the QList does not delete the Mesh object being pointed too
    // meshList.clear();
}

void View::AddMaterialData(std::vector<float> materialData)
{
    materials = materialData;
}

void View::AddTexture(Texture *texture)
{
    textureList.append(texture);
}

void View::AddCamera(Camera *cam)
{
    camera = cam;
}

void View::AddShader(Shader *shad)
{
    shader = shad;
}

void View::AddPickingShader(Shader *shad)
{
    pickingShader = shad;
}

void View::AddEdgeShader(Shader *shad)
{
    edgeShader = shad;
}

void View::SetProjection(QMatrix4x4 &projection)
{
    m_projectionMatrix = projection;
}

void View::SetWidth(int width)
{
    viewportWidth = width;
}

void View::SetHeight(int height)
{
    viewportHeight = height;
}

void View::SetDefaultFBO(GLuint fbo)
{
    defaultFBO = fbo;
}

void View::SetSelectionCoordinates(int x, int y)
{
    m_pickX = x;
    m_pickY = y;
}

void View::encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b)
{
    r = (id & 0x000000FF);
    g = (id & 0x0000FF00) >> 8;
    b = (id & 0x00FF0000) >> 16;
}

void View::ensurePickFBO()
{
    if (m_pickFBO != 0) {
        this->glDeleteFramebuffers(1, &m_pickFBO);
        this->glDeleteTextures(1, &m_pickColorTex);
        this->glDeleteRenderbuffers(1, &m_pickDepthBuf);
        m_pickFBO = m_pickColorTex = m_pickDepthBuf = 0;
    }

    // Generate framebuffer
    this->glGenFramebuffers(1, &m_pickFBO);
    this->glBindFramebuffer(GL_FRAMEBUFFER, m_pickFBO);

    // --- Color attachment (texture) ---
    this->glGenTextures(1, &m_pickColorTex);
    this->glBindTexture(GL_TEXTURE_2D, m_pickColorTex);
    this->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, viewportWidth, viewportHeight, 0,
                       GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    this->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    this->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                 GL_TEXTURE_2D, m_pickColorTex, 0);

    // --- Depth attachment (renderbuffer) ---
    this->glGenRenderbuffers(1, &m_pickDepthBuf);
    this->glBindRenderbuffer(GL_RENDERBUFFER, m_pickDepthBuf);
    this->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, viewportWidth, viewportHeight);
    this->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                    GL_RENDERBUFFER, m_pickDepthBuf);

    // Check FBO status
    if (this->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qWarning() << "Picking FBO is not complete!";
    }

    // Unbind
    this->glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
}

void View::InitializeHandles()
{
    //
    /*
        Static VBOs and IBOs
    */
    this->glGenVertexArrays(1, &m_vao);
    this->glGenVertexArrays(1, &m_edge_vao);
    this->glGenBuffers(1, &m_static_ibo);

    this->glGenBuffers(1, &m_static_position_vbo);
    this->glGenBuffers(1, &m_static_normal_vbo);
    this->glGenBuffers(1, &m_static_textureuv_vbo);
    this->glGenBuffers(1, &m_static_materialIndex_vbo);
    this->glGenBuffers(1, &m_static_textureIndex_vbo);

    this->glGenBuffers(1, &m_static_corner_vbo);
    this->glGenBuffers(1, &m_static_edge_indices_vbo);
    this->glGenBuffers(1, &m_static_edge_width_vbo);
    this->glGenBuffers(1, &m_static_edge_dashLength_vbo);
    this->glGenBuffers(1, &m_static_edge_gapLength_vbo);
    this->glGenBuffers(1, &m_static_edge_dash_vbo);
    this->glGenBuffers(1, &m_static_edge_materialIndex_vbo);

    this->glGenBuffers(1, &m_static_border_ibo);
    this->glGenBuffers(1, &m_static_matrix_tbo);
    this->glGenBuffers(1, &m_static_model_matrix_vbo);
    this->glGenTextures(1, &m_static_matrixTexture);
    this->glGenTextures(1, &m_static_verticesTexture);
    this->glGenTextures(1, &m_static_matrixIndexTexture);

    /*
        Dynmic VBOs and IBOs
    */
    this->glGenVertexArrays(1, &m_dynamic_vao);
    this->glGenVertexArrays(1, &m_dynamic_edge_vao);
    this->glGenBuffers(1, &m_dynamic_ibo);

    this->glGenBuffers(1, &m_dynamic_position_vbo);
    this->glGenBuffers(1, &m_dynamic_normal_vbo);
    this->glGenBuffers(1, &m_dynamic_textureuv_vbo);
    this->glGenBuffers(1, &m_dynamic_materialIndex_vbo);
    this->glGenBuffers(1, &m_dynamic_textureIndex_vbo);

    this->glGenBuffers(1, &m_dynamic_corner_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_indices_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_width_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_dashLength_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_gapLength_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_dash_vbo);
    this->glGenBuffers(1, &m_dynamic_edge_materialIndex_vbo);

    this->glGenBuffers(1, &m_dynamic_border_ibo);
    this->glGenBuffers(1, &m_dynamic_matrix_tbo);
    this->glGenBuffers(1, &m_dynamic_model_matrix_vbo);
    this->glGenTextures(1, &m_dynamic_matrixTexture);
    this->glGenTextures(1, &m_dynamic_verticesTexture);
    this->glGenTextures(1, &m_dynamic_matrixIndexTexture);

    /*
        Texture Buffers
    */
    this->glGenBuffers(1, &m_material_tbo);
    this->glGenTextures(1, &m_materialTexture);
    this->glGenBuffers(1, &m_pick_color_vbo);
}

void View::InitializeStaticBuffers()
{
    // For now num of verticies is 1000
    int numOfVertices = 100000;
    int numOfIndices = 100000;
    int numOfEdges = 100000;
    int numOfModelMatrices = 1000;
    int numOfModelMatrixIndices = 100000;
    std::array<float, 4> corners = {-1.0f, 1.0f, -1.0f, 1.0f};

    // VAO for Mesh + Mesh Color Picking
    this->glBindVertexArray(m_vao);
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_position_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * numOfVertices, NULL, GL_STATIC_DRAW);
            // postition in verticies
            this->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Position), (void*)0);
            this->glEnableVertexAttribArray(0);


        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_normal_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * numOfVertices, NULL, GL_STATIC_DRAW);
            // normal in verticies
            this->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);
            this->glEnableVertexAttribArray(1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureuv_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(TextureUV) * numOfVertices, NULL, GL_STATIC_DRAW);
            // texture uv in verticies
            this->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextureUV), (void*)0);
            this->glEnableVertexAttribArray(2);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVertices, NULL, GL_STATIC_DRAW);
            // materialIndex in verticies
            this->glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(3);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_textureIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVertices, NULL, GL_STATIC_DRAW);
            // textureIndex in verticies
            this->glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(4);

        // this->glBindBuffer(GL_ARRAY_BUFFER, 0);
        // this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_model_matrix_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfModelMatrixIndices, NULL, GL_STATIC_DRAW);

            this->glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(5);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_pick_color_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(std::array<float, 4>) * numOfVertices, NULL, GL_STATIC_DRAW);

            // pickColor Values
            this->glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(std::array<float, 4>), (void*)0);
            this->glEnableVertexAttribArray(6);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_static_ibo);
            this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfIndices, NULL, GL_STATIC_DRAW);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_static_matrix_tbo);
            this->glBufferData(GL_TEXTURE_BUFFER, numOfModelMatrices * sizeof(float), NULL, GL_STATIC_DRAW);
        this->glBindBuffer(GL_TEXTURE_BUFFER, 0);

        this->glBindTexture(GL_TEXTURE_BUFFER, m_static_matrixTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_static_matrix_tbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);

        // TBO for Materials
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_material_tbo);
            this->glBufferData(GL_TEXTURE_BUFFER, materials.size() * sizeof(float), materials.data(), GL_STATIC_DRAW);
        this->glBindBuffer(GL_TEXTURE_BUFFER, 0);

        this->glBindTexture(GL_TEXTURE_BUFFER, m_materialTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_material_tbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);

    this->glBindVertexArray(0);

    // VAO for edge
    this->glBindVertexArray(m_edge_vao);
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_corner_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, corners.data(), GL_STATIC_DRAW);
            // corners
            this->glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(0);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_indices_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(EdgeIndex) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edges
            this->glVertexAttribIPointer(1, 2, GL_INT, sizeof(EdgeIndex), (void*)0);
            this->glEnableVertexAttribArray(1);
            this->glVertexAttribDivisor(1, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_width_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edge width
            this->glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(2);
            this->glVertexAttribDivisor(2, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edge material index
            this->glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(3);
            this->glVertexAttribDivisor(3, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dashLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edge dash length
            this->glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(4);
            this->glVertexAttribDivisor(4, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_gapLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edge gap length
            this->glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(5);
            this->glVertexAttribDivisor(5, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_static_edge_dash_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdges, NULL, GL_STATIC_DRAW);
            // edge dash
            this->glVertexAttribIPointer(6, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(6);
            this->glVertexAttribDivisor(6, 1);


        // TBO for Vertices
        this->glBindTexture(GL_TEXTURE_BUFFER, m_static_verticesTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_static_position_vbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);

        // TBO for Matrix Index
        this->glBindTexture(GL_TEXTURE_BUFFER, m_static_matrixIndexTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, m_static_model_matrix_vbo);
            this->glBindTexture(GL_TEXTURE_BUFFER, 0);
    this->glBindVertexArray(0);


    // Binding TBO to texture units
    // For texture arrays
    this->glActiveTexture(GL_TEXTURE0);
        this->glBindTexture(GL_TEXTURE_2D_ARRAY, textureList[0]->getTextureId());

    // For Material List
    this->glActiveTexture(GL_TEXTURE1);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_materialTexture);

    // For Model Matrices
    this->glActiveTexture(GL_TEXTURE2);
    this->glBindTexture(GL_TEXTURE_BUFFER, m_static_matrixTexture);

    // For vertices arrays
    this->glActiveTexture(GL_TEXTURE3);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_static_verticesTexture);

    // For Model Matrix Indices
    this->glActiveTexture(GL_TEXTURE4);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_static_matrixIndexTexture);
}

void View::InitializeDynamicBuffers()
{
    // For now num of verticies is 1000
    int numOfVertices = 100000;
    int numOfIndices = 100000;
    int numOfEdges = 100000;
    int numOfModelMatrices = 1000;
    int numOfModelMatrixIndices = 100000;
    std::array<float, 4> corners = {-1.0f, 1.0f, -1.0f, 1.0f};

    // VAO for Mesh + Mesh Color Picking
    this->glBindVertexArray(m_dynamic_vao);
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_position_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Position) * numOfVertices, NULL, GL_STREAM_DRAW);
            // postition in verticies
            this->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Position), (void*)0);
            this->glEnableVertexAttribArray(0);


        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_normal_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(Normal) * numOfVertices, NULL, GL_STREAM_DRAW);
            // normal in verticies
            this->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), (void*)0);
            this->glEnableVertexAttribArray(1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureuv_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(TextureUV) * numOfVertices, NULL, GL_STREAM_DRAW);
            // texture uv in verticies
            this->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TextureUV), (void*)0);
            this->glEnableVertexAttribArray(2);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVertices, NULL, GL_STREAM_DRAW);
            // materialIndex in verticies
            this->glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(3);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_textureIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfVertices, NULL, GL_STREAM_DRAW);
            // textureIndex in verticies
            this->glVertexAttribIPointer(4, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(4);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_model_matrix_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfModelMatrixIndices, NULL, GL_STREAM_DRAW);

            this->glVertexAttribIPointer(5, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(5);


        //TRIANGLE IBO
        this->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_dynamic_ibo);
            this->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numOfIndices, NULL, GL_STREAM_DRAW);

        // TBO for Model Matrix
        this->glBindBuffer(GL_TEXTURE_BUFFER, m_dynamic_matrix_tbo);
            this->glBufferData(GL_TEXTURE_BUFFER, numOfModelMatrices * sizeof(float), NULL, GL_STREAM_DRAW);
        this->glBindBuffer(GL_TEXTURE_BUFFER, 0);

        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_matrixTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_dynamic_matrix_tbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);

        // // TBO for Materials
        // this->glBindBuffer(GL_TEXTURE_BUFFER, m_material_tbo);
        //     this->glBufferData(GL_TEXTURE_BUFFER, materials.size() * sizeof(float), materials.data(), GL_STREAM_DRAW);
        // this->glBindBuffer(GL_TEXTURE_BUFFER, 0);

        // this->glBindTexture(GL_TEXTURE_BUFFER, m_materialTexture);
        //     this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_material_tbo);
        // this->glBindTexture(GL_TEXTURE_BUFFER, 0);

    this->glBindVertexArray(0);

    // VAO for edge
    this->glBindVertexArray(m_dynamic_edge_vao);
        //VBO
        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_corner_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4, corners.data(), GL_STREAM_DRAW);
            // corners
            this->glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(0);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_indices_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(EdgeIndex) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edges
            this->glVertexAttribIPointer(1, 2, GL_INT, sizeof(EdgeIndex), (void*)0);
            this->glEnableVertexAttribArray(1);
            this->glVertexAttribDivisor(1, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_width_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edge width
            this->glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(2);
            this->glVertexAttribDivisor(2, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_materialIndex_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edge material index
            this->glVertexAttribIPointer(3, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(3);
            this->glVertexAttribDivisor(3, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dashLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edge dash length
            this->glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(4);
            this->glVertexAttribDivisor(4, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_gapLength_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edge gap length
            this->glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
            this->glEnableVertexAttribArray(5);
            this->glVertexAttribDivisor(5, 1);

        this->glBindBuffer(GL_ARRAY_BUFFER, m_dynamic_edge_dash_vbo);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(int) * numOfEdges, NULL, GL_STREAM_DRAW);
            // edge dash
            this->glVertexAttribIPointer(6, 1, GL_INT, sizeof(int), (void*)0);
            this->glEnableVertexAttribArray(6);
            this->glVertexAttribDivisor(6, 1);


        // TBO for Vertices
        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_verticesTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, m_dynamic_position_vbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);

        // TBO for Matrix Index
        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_matrixIndexTexture);
            this->glTexBuffer(GL_TEXTURE_BUFFER, GL_R32I, m_dynamic_model_matrix_vbo);
        this->glBindTexture(GL_TEXTURE_BUFFER, 0);
    this->glBindVertexArray(0);



    // Binding TBO to texture units
    // For Model Matrices
    this->glActiveTexture(GL_TEXTURE5);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_matrixTexture);

    // For vertices arrays
    this->glActiveTexture(GL_TEXTURE6);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_verticesTexture);

    // For Model Matrix Indices
    this->glActiveTexture(GL_TEXTURE7);
        this->glBindTexture(GL_TEXTURE_BUFFER, m_dynamic_matrixIndexTexture);
}

View::~View()
{
    //
    /*
        Static VBO and IBO
    */
    this->glDeleteBuffers(1, &m_static_position_vbo);
    this->glDeleteBuffers(1, &m_static_normal_vbo);
    this->glDeleteBuffers(1, &m_static_textureuv_vbo);
    this->glDeleteBuffers(1, &m_static_materialIndex_vbo);
    this->glDeleteBuffers(1, &m_static_textureIndex_vbo);

    this->glDeleteBuffers(1, &m_static_corner_vbo);
    this->glDeleteBuffers(1, &m_static_edge_indices_vbo);
    this->glDeleteBuffers(1, &m_static_edge_width_vbo);
    this->glDeleteBuffers(1, &m_static_edge_dashLength_vbo);
    this->glDeleteBuffers(1, &m_static_edge_gapLength_vbo);
    this->glDeleteBuffers(1, &m_static_edge_dash_vbo);
    this->glDeleteBuffers(1, &m_static_edge_materialIndex_vbo);

    this->glDeleteBuffers(1, &m_static_ibo);
    this->glDeleteBuffers(1, &m_static_border_ibo);
    this->glDeleteBuffers(1, &m_static_matrix_tbo);
    this->glDeleteTextures(1, &m_static_matrixTexture);
    this->glDeleteTextures(1, &m_static_verticesTexture);
    this->glDeleteTextures(1, &m_static_matrixIndexTexture);
    this->glDeleteBuffers(1, &m_static_model_matrix_vbo);


    /*
        Dynamic VBO and IBO
    */
    this->glDeleteBuffers(1, &m_dynamic_position_vbo);
    this->glDeleteBuffers(1, &m_dynamic_normal_vbo);
    this->glDeleteBuffers(1, &m_dynamic_textureuv_vbo);
    this->glDeleteBuffers(1, &m_dynamic_materialIndex_vbo);
    this->glDeleteBuffers(1, &m_dynamic_textureIndex_vbo);

    this->glDeleteBuffers(1, &m_dynamic_corner_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_indices_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_width_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_dashLength_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_gapLength_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_dash_vbo);
    this->glDeleteBuffers(1, &m_dynamic_edge_materialIndex_vbo);

    this->glDeleteBuffers(1, &m_dynamic_ibo);
    this->glDeleteBuffers(1, &m_dynamic_border_ibo);
    this->glDeleteBuffers(1, &m_dynamic_matrix_tbo);
    this->glDeleteTextures(1, &m_dynamic_matrixTexture);
    this->glDeleteTextures(1, &m_dynamic_verticesTexture);
    this->glDeleteTextures(1, &m_dynamic_matrixIndexTexture);
    this->glDeleteBuffers(1, &m_dynamic_model_matrix_vbo);

    /*
        Textures
    */
    this->glDeleteBuffers(1, &m_material_tbo);
    this->glDeleteTextures(1, &m_materialTexture);
    this->glDeleteBuffers(1, &m_pick_color_vbo);


    /*
        VAOs
    */
    this->glDeleteVertexArrays(1, &m_vao);
    this->glDeleteVertexArrays(1, &m_edge_vao);
    this->glDeleteVertexArrays(1, &m_dynamic_vao);
    this->glDeleteVertexArrays(1, &m_dynamic_edge_vao);

    delete combinedMesh;
}
