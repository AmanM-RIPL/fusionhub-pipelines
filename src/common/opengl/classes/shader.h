#ifndef SHADER_H
#define SHADER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QOpenGLFunctions_3_3_Core>

class Shader : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Shader(QObject *parent = nullptr);
    ~Shader();

    enum ShaderType { MESH, MESH_COLOR_PICKING, EDGE };

    void CreateFromFiles(const QString& vertexLocation, const QString& fragmentLocation);
    QByteArray ReadFile(const QString& fileLocation);

    GLuint getShaderId();

    GLuint getProjectionId();
    GLuint getViewId();

    GLuint getPickColorId();
    GLuint getViewPositionId();

    GLuint getMaterialAmbientId(int materialIndex);
    GLuint getMaterialDiffuseId(int materialIndex);
    GLuint getMaterialSpecularId(int materialIndex);
    GLuint getMaterialShininessId(int materialIndex);

    GLuint getTextureArrayId();
    int* getTextureUnitArray();

    GLuint getLightPositionId();
    GLuint getLightAmbientId();
    GLuint getLightDiffuseId();
    GLuint getLightSpecularId();

    GLuint getModelMatrixBufferId();
    GLuint getMaterialBufferId();

    void SetShaderType(ShaderType value);

private:
    GLuint shaderID,
        m_projectionId, m_viewId,
        m_pickColorId, m_viewPositionId,
        m_lightPositionId, m_lightAmbientId, m_lightDiffuseId, m_lightSpecularId,
        m_modelMatrixBufferId, m_materialBufferId,
        m_viewportId, m_verticesId, m_modelMatrixIndexBufferId; // for edge shader only

    GLuint m_textureArrayId;
    int m_textureCount = 1;
    int textureArray[1] = {0};

    int m_materialCount = 2;

    struct
    {
        GLint  m_materialAmbientId;
        GLint  m_materialDiffuseId;
        GLint  m_materialSpecularId;
        GLint  m_materialShininessId;
    } m_materialId[2];

    ShaderType m_shaderType = Shader::MESH;

    void AddShader(GLenum type, const char* src);
    void CompileShader(const char* vertexCode, const char* fragmentCode);

signals:
};

#endif // SHADER_H
