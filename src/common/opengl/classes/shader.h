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

    GLuint getMaterialAmbientId();
    GLuint getMaterialDiffuseId();
    GLuint getMaterialSpecularId();
    GLuint getMaterialShininessId();

    GLuint getLightPositionId();
    GLuint getLightAmbientId();
    GLuint getLightDiffuseId();
    GLuint getLightSpecularId();

    void SetPickColor(bool value);

private:
    GLuint shaderID,
        m_projectionId, m_modelId, m_viewId,
        m_pickColorId, m_viewPositionId,
        m_materialAmbientId, m_materialDiffuseId, m_materialSpecularId, m_materialShininessId,
        m_lightPositionId, m_lightAmbientId, m_lightDiffuseId, m_lightSpecularId;

    bool setPickColorId = false;

    void AddShader(GLenum type, const char* src);
    void CompileShader(const char* vertexCode, const char* fragmentCode);

signals:
};

#endif // SHADER_H
