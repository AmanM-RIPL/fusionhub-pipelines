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

    void CreateFromFiles(const QString& vertexLocation, const QString& fragmentLocation);
    QByteArray ReadFile(const QString& fileLocation);

    GLuint getShaderId();

    GLuint getProjectionId();
    GLuint getModelId();
    GLuint getViewId();

    GLuint getPickColorId();
    GLuint getViewPositionId();

    GLuint getMaterialAmbientId(int materialIndex);
    GLuint getMaterialDiffuseId(int materialIndex);
    GLuint getMaterialSpecularId(int materialIndex);
    GLuint getMaterialShininessId(int materialIndex);

    GLuint getLightPositionId();
    GLuint getLightAmbientId();
    GLuint getLightDiffuseId();
    GLuint getLightSpecularId();

    void SetPickColor(bool value);

private:
    GLuint shaderID,
        m_projectionId, m_modelId, m_viewId,
        m_pickColorId, m_viewPositionId,
        m_lightPositionId, m_lightAmbientId, m_lightDiffuseId, m_lightSpecularId;

    int m_materialCount = 1;

    struct
    {
        GLint  m_materialAmbientId;
        GLint  m_materialDiffuseId;
        GLint  m_materialSpecularId;
        GLint  m_materialShininessId;
    } m_materialId[1];

    bool setPickColorId = false;

    void AddShader(GLenum type, const char* src);
    void CompileShader(const char* vertexCode, const char* fragmentCode);

signals:
};

#endif // SHADER_H
