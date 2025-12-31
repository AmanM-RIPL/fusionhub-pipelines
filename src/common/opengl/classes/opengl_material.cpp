#include "opengl_material.h"

OpenGLMaterial::OpenGLMaterial(QObject *parent)
    : QObject{parent}
{}

void OpenGLMaterial::GenerateMaterialList(QList<OpenGLMaterial *> &material_list)
{
    // No need to handle OpenGLMaterial Desctructor here
    // The lifecycle is handled in myglitem.h (check ~MyGLRenderer())

    // for IVORY
    OpenGLMaterial* whiteMaterial = new OpenGLMaterial();
    whiteMaterial->setAmbient({1.0f, 1.0f, 1.0f}); // 0.96, 0.47f, 0.02f
    whiteMaterial->setDiffuse({1.0f, 1.0f, 1.0f}); // 0.0f, 0.5f, 0.31f
    whiteMaterial->setSpecular({1.0f, 1.0f, 1.0f}); // 0.5f, 0.5f, 0.5f
    whiteMaterial->setShininess(32.0f);
    material_list.append(whiteMaterial);

    // For BLUE
    OpenGLMaterial* blueMaterial = new OpenGLMaterial();
    blueMaterial->setAmbient({0.68f, 0.85f, 0.90f}); // 0.96, 0.47f, 0.02f
    blueMaterial->setDiffuse({1.0f, 1.0f, 1.0f}); // 0.0f, 0.5f, 0.31f
    blueMaterial->setSpecular({1.0f, 1.0f, 1.0f}); // 0.5f, 0.5f, 0.5f
    blueMaterial->setShininess(32.0f);
    material_list.append(blueMaterial);
}

std::vector<float> OpenGLMaterial::GenerateMaterialData(QList<OpenGLMaterial *> &material_list)
{
    std::vector<float> materialData;

    for (OpenGLMaterial* mat : material_list) {
        std::array<float, 3> ambient = mat->ambient();
        std::array<float, 3> diffuse = mat->diffuse();
        std::array<float, 3> specular = mat->specular();
        float shininess = mat->shininess();

        materialData.push_back(ambient[0]);  // 1
        materialData.push_back(ambient[1]);  // 2
        materialData.push_back(ambient[2]);  // 3
        materialData.push_back(diffuse[0]);  // 4 -> Texel 0 end

        materialData.push_back(diffuse[1]);  // 5
        materialData.push_back(diffuse[2]);  // 6
        materialData.push_back(specular[0]); // 7
        materialData.push_back(specular[1]); // 8 -> Texel 1 end

        materialData.push_back(specular[2]); // 9
        materialData.push_back(shininess);  // 10
        materialData.push_back(0.0f);           // 11 (Padding)
        materialData.push_back(0.0f);           // 12 (Padding) -> Texel 2 end
    }

    //padding of two required as texelFetch in shader is always vec4

    return materialData;
}

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
