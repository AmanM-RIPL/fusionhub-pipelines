#include "shader.h"

Shader::Shader(QObject *parent)
    : QObject{parent}
{}

void Shader::CreateFromFiles(const QString& vertexLocation, const QString& fragmentLocation)
{
    // initialize opengl
    this->initializeOpenGLFunctions();

    // create shaders in opnegl
    QByteArray vertexByteArray = ReadFile(vertexLocation);
    QByteArray fragmentByteArray = ReadFile(fragmentLocation);

    const char* vertexCode = vertexByteArray.constData();
    const char* fragmentCode = fragmentByteArray.constData();

    CompileShader(vertexCode, fragmentCode);
}

QByteArray Shader::ReadFile(const QString& fileLocation)
{
    QFile file(fileLocation);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Could not open file:" << fileLocation;
        return QByteArray(); // Or handle the error appropriately
    }

    QByteArray fileContent = file.readAll();
    QString contentString = QString::fromUtf8(fileContent);

    return contentString.toUtf8();
}

void Shader::AddShader(GLenum type, const char* src) {
    GLuint shader = this->glCreateShader(type);
    this->glShaderSource(shader, 1, &src, nullptr);
    this->glCompileShader(shader);

    GLint success;
    this->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        this->glGetShaderInfoLog(shader, 512, nullptr, log);
        qWarning() << "Shader compile error:" << log;
    }


    this->glAttachShader(shaderID, shader);
    this->glDeleteShader(shader);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = this->glCreateProgram();
    if (!shaderID)
    {
        qInfo() << "Failed to create shader";
        return;
    }

    AddShader(GL_VERTEX_SHADER, vertexCode);
    AddShader(GL_FRAGMENT_SHADER, fragmentCode);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    this->glLinkProgram(shaderID);
    this->glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        this->glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        qInfo() << eLog;
        return;
    }

    this->glValidateProgram(shaderID);
    this->glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        this->glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        qInfo() << eLog;
        return;
    }

    m_modelId = this->glGetUniformLocation(shaderID, "model");
    m_projectionId = this->glGetUniformLocation(shaderID, "projection");
    m_viewId = this->glGetUniformLocation(shaderID, "view");

    if (setPickColorId)
    {
        m_pickColorId = this->glGetUniformLocation(shaderID, "pickColor");
    }
    else
    {
        m_viewPositionId = this->glGetUniformLocation(shaderID, "viewPosition");

        m_materialAmbientId = this->glGetUniformLocation(shaderID, "material.ambient");
        m_materialDiffuseId = this->glGetUniformLocation(shaderID, "material.diffuse");
        m_materialSpecularId = this->glGetUniformLocation(shaderID, "material.specular");
        m_materialShininessId = this->glGetUniformLocation(shaderID, "material.shininess");

        m_lightPositionId = this->glGetUniformLocation(shaderID, "light.position");
        m_lightAmbientId = this->glGetUniformLocation(shaderID, "light.ambient");
        m_lightDiffuseId = this->glGetUniformLocation(shaderID, "light.diffuse");
        m_lightSpecularId = this->glGetUniformLocation(shaderID, "light.specular");
    }
}

GLuint Shader::getShaderId()
{
    return shaderID;
}

GLuint Shader::getProjectionId()
{
    return m_projectionId;
}

GLuint Shader::getModelId()
{
    return m_modelId;
}

GLuint Shader::getViewId()
{
    return m_viewId;
}

GLuint Shader::getPickColorId()
{
    return m_pickColorId;
}

GLuint Shader::getViewPositionId()
{
    return m_viewPositionId;
}

GLuint Shader::getMaterialAmbientId()
{
    return m_materialAmbientId;
}

GLuint Shader::getMaterialDiffuseId()
{
    return m_materialDiffuseId;
}

GLuint Shader::getMaterialSpecularId()
{
    return m_materialSpecularId;
}

GLuint Shader::getMaterialShininessId()
{
    return m_materialShininessId;
}

GLuint Shader::getLightPositionId()
{
    return m_lightPositionId;
}

GLuint Shader::getLightAmbientId()
{
    return m_lightAmbientId;
}

GLuint Shader::getLightDiffuseId()
{
    return m_lightDiffuseId;
}

GLuint Shader::getLightSpecularId()
{
    return m_lightSpecularId;
}

void Shader::SetPickColor(bool value)
{
    setPickColorId = value;
}

Shader::~Shader()
{
    this->glDeleteProgram(shaderID);
}
