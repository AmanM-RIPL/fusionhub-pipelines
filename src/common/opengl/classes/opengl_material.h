#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include <QObject>
#include <QList>
#include <array>


class OpenGLMaterial : public QObject
{
    Q_OBJECT
public:
    explicit OpenGLMaterial(QObject *parent = nullptr);

    enum MaterialName { IVORY, BLUE, BLACK }; // before changing order make sure to update GenerateMaterialList

    static void GenerateMaterialList(QList<OpenGLMaterial*>& material_list);
    static std::vector<float> GenerateMaterialData(QList<OpenGLMaterial*>& material_list); // for sending to OpenGL

    std::array<float, 3> ambient() const;
    void setAmbient(const std::array<float, 3> &newAmbient);

    std::array<float, 3> diffuse() const;
    void setDiffuse(const std::array<float, 3> &newDiffuse);

    std::array<float, 3> specular() const;
    void setSpecular(const std::array<float, 3> &newSpecular);

    float shininess() const;
    void setShininess(float newShininess);

signals:

private:
    std::array<float, 3> m_ambient;
    std::array<float, 3> m_diffuse;
    std::array<float, 3> m_specular;
    float m_shininess;
};

#endif // OPENGL_MATERIAL_H
