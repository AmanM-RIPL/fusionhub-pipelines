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

void Shader::SetPickColor(bool value)
{
    setPickColorId = value;
}

Shader::~Shader()
{
    this->glDeleteProgram(shaderID);
}
