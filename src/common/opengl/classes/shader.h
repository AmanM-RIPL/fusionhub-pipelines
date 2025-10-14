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

    void SetPickColor(bool value);

private:
    GLuint shaderID, m_projectionId, m_modelId, m_viewId, m_pickColorId;
    bool setPickColorId = false;

    void AddShader(GLenum type, const char* src);
    void CompileShader(const char* vertexCode, const char* fragmentCode);

signals:
};

#endif // SHADER_H
