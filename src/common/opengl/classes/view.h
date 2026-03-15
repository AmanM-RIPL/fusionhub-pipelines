#ifndef VIEW_H
#define VIEW_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVector4D>
#include <QList>

#include "mesh.h"
#include "camera.h"
#include "shader.h"
#include "opengl_material.h"
#include "texture.h"
#include "mesh_map.h"

struct Ray
{
    QVector3D position;
    QVector3D direction;
};

class View : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    ~View();

    void Initialize();
    void Render();

    void LoadStaticMeshData(MeshMap* meshMap);
    void AppendToStaticMeshData(MeshMap* meshMap);
    void LoadStaticIndicesData(MeshMap* meshMap, ViewType view_type);
    void LoadDynamicMeshData(Mesh* mesh);
    // void UpdateTransformations(); // Update Camera, Light, View, Projection UBO
    // void UpdateStaticMeshData(Mesh* mesh, int meshIndex);

    unsigned int Selection();
    QVector3D GetPointInModelSpace(Mesh* mesh);
    QVector3D GetPointInViewSpace(int pointX, int pointY);
    Ray GetRayFromCamera(int pointX, int pointY);
    std::array<float, 2> GetPointInScreenSpace(QVector3D& point3D);

    void AddMesh(Mesh* mesh);
    void DeleteAllMesh(); // will clear the QList but will not delete the Mesh pointer
    void AddMaterialData(std::vector<float> materialData);
    void AddTexture(Texture* texture);
    void AddCamera(Camera* cam);
    void AddShader(Shader* shad);
    void AddPickingShader(Shader* shad);
    void AddEdgeShader(Shader* shad);

    void SetProjection(QMatrix4x4& projection);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetDefaultFBO(GLuint fbo);
    void SetSelectionCoordinates(int x, int y);

private:
    GLuint m_vao = 0;
    GLuint m_edge_vao = 0;
    GLuint m_dynamic_vao = 0; // for the bim entity being edited
    GLuint m_dynamic_edge_vao = 0; // for the bim entity being edited

    /*
        Static VBOs and IBOs
    */
    GLuint m_static_position_vbo = 0;
    GLuint m_static_normal_vbo = 0;
    GLuint m_static_textureuv_vbo = 0;
    GLuint m_static_materialIndex_vbo = 0;
    GLuint m_static_textureIndex_vbo = 0;

    GLuint m_static_corner_vbo = 0;
    GLuint m_static_edge_indices_vbo = 0;
    GLuint m_static_edge_data_int_vbo = 0;
    GLuint m_static_edge_data_float_vbo = 0;

    GLuint m_static_ibo = 0;
    GLuint m_static_border_ibo = 0;

    GLuint m_static_matrix_tbo = 0;
    GLuint m_static_model_matrix_vbo = 0;

    GLuint m_static_matrixTexture = 0;
    GLuint m_static_verticesTexture = 0;
    GLuint m_static_matrixIndexTexture = 0;
    GLuint m_static_edgeDataIntTexture = 0;
    GLuint m_static_edgeDataFloatTexture = 0;


    /*
        Dynamic VBOs and IBOs
    */
    GLuint m_dynamic_position_vbo = 0;
    GLuint m_dynamic_normal_vbo = 0;
    GLuint m_dynamic_textureuv_vbo = 0;
    GLuint m_dynamic_materialIndex_vbo = 0;
    GLuint m_dynamic_textureIndex_vbo = 0;

    GLuint m_dynamic_corner_vbo = 0;
    GLuint m_dynamic_edge_indices_vbo = 0;
    GLuint m_dynamic_edge_data_int_vbo = 0;
    GLuint m_dynamic_edge_data_float_vbo = 0;


    GLuint m_dynamic_ibo = 0;
    GLuint m_dynamic_border_ibo = 0;

    GLuint m_dynamic_matrix_tbo = 0;
    GLuint m_dynamic_model_matrix_vbo = 0;

    GLuint m_dynamic_matrixTexture = 0;
    GLuint m_dynamic_verticesTexture = 0;
    GLuint m_dynamic_matrixIndexTexture = 0;
    GLuint m_dynamic_edgeDataIntTexture = 0;
    GLuint m_dynamic_edgeDataFloatTexture = 0;

    /*
        Texture Buffer Objects (Common)
    */
    GLuint m_material_tbo = 0;
    GLuint m_materialTexture = 0;
    GLuint m_pick_color_vbo = 0;

    // for color picking
    int m_pickX = -1;
    int m_pickY = -1;

    GLuint m_pickFBO = 0;
    GLuint m_pickColorTex = 0;
    GLuint m_pickDepthBuf = 0;

    GLsizei m_static_indexCount;
    GLsizei m_static_borderIndexCount;
    GLsizei m_dynamic_indexCount;
    GLsizei m_dynamic_borderIndexCount;

    QMatrix4x4 m_projectionMatrix = QMatrix4x4();
    GLuint defaultFBO = 0;

    std::vector<float> materials;
    QList<Texture*> textureList;
    CombinedMesh* combinedMesh = nullptr; // need to delete it with view only!!
    CombinedIndices* combinedIndices = nullptr; // need to delete it with view only!!

    int viewportWidth = 0;
    int viewportHeight = 0;

    Camera* camera;
    Shader* shader;
    Shader* pickingShader;
    Shader* edgeShader;

    // For Color Picking
    void encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b);
    void ensurePickFBO();

    void BindBuffers();

    void InitializeHandles();
    void InitializeStaticBuffers(); // Initialize VBO + TBO
    void InitializeDynamicBuffers(); // Initialize VBO + TBO
};

#endif // VIEW_H
