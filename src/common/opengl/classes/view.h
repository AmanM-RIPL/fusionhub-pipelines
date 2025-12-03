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

class View : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit View(QObject *parent = nullptr);
    ~View();

    void Initialize();
    void BindMeshWithOpenGL();
    void Render();

    void Selection();
    QVector3D GetPointInModelSpace(int meshIndex);
    QVector3D GetPointInViewSpace();

    void AddMesh(Mesh* mesh);
    void DeleteAllMesh(); // will clear the QList but will not delete the Mesh pointer
    void AddMaterial(OpenGLMaterial* material);
    void AddTexture(Texture* texture);
    void AddCamera(Camera* cam);
    void AddShader(Shader* shad);
    void AddPickingShader(Shader* shad);

    void SetProjection(QMatrix4x4& projection);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetDefaultFBO(GLuint fbo);
    void SetSelectionCoordinates(int x, int y);

private:
    GLuint m_vao = 0;
    GLuint m_static_vbo = 0;
    GLuint m_editor_vbo = 0;
    GLuint m_static_ibo = 0;
    GLuint m_editor_ibo = 0;
    GLuint m_static_border_ibo = 0;
    GLuint m_tbo = 0;
    GLuint m_matrixTexture = 0;
    GLuint m_model_matrix_vbo = 0;

    // for color picking
    int m_pickX = -1;
    int m_pickY = -1;

    GLuint m_pickFBO = 0;
    GLuint m_pickColorTex = 0;
    GLuint m_pickDepthBuf = 0;

    GLsizei m_indexCount;
    GLsizei m_borderIndexCount;

    QMatrix4x4 m_projectionMatrix = QMatrix4x4();
    GLuint defaultFBO = 0;

    QList<Mesh*> meshList;
    QList<OpenGLMaterial*> materialList;
    QList<Texture*> textureList;
    Mesh* combinedMesh = nullptr; // need to delete it with view only!!

    int viewportWidth = 0;
    int viewportHeight = 0;

    Camera* camera;
    Shader* shader;
    Shader* pickingShader;

    // For Color Picking
    void encodeIdToColor(unsigned int id, unsigned char &r, unsigned char &g, unsigned char &b);
    void ensurePickFBO();

    void BindBuffers();
};

#endif // VIEW_H
