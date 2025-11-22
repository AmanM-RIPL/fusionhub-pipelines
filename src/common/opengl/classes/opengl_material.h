#ifndef OPENGL_MATERIAL_H
#define OPENGL_MATERIAL_H

#include <QObject>
#include <array>

class OpenGLMaterial : public QObject
{
    Q_OBJECT
public:
    explicit OpenGLMaterial(QObject *parent = nullptr);

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
