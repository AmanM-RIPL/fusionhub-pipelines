#include "opengl_material.h"

OpenGLMaterial::OpenGLMaterial(QObject *parent)
    : QObject{parent}
{}

std::array<float, 3> OpenGLMaterial::ambient() const
{
    return m_ambient;
}

void OpenGLMaterial::setAmbient(const std::array<float, 3> &newAmbient)
{
    m_ambient = newAmbient;
}

std::array<float, 3> OpenGLMaterial::diffuse() const
{
    return m_diffuse;
}

void OpenGLMaterial::setDiffuse(const std::array<float, 3> &newDiffuse)
{
    m_diffuse = newDiffuse;
}


std::array<float, 3> OpenGLMaterial::specular() const
{
    return m_specular;
}

void OpenGLMaterial::setSpecular(const std::array<float, 3> &newSpecular)
{
    m_specular = newSpecular;
}

float OpenGLMaterial::shininess() const
{
    return m_shininess;
}

void OpenGLMaterial::setShininess(float newShininess)
{
    m_shininess = newShininess;
}
