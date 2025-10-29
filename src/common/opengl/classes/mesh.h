#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>

#include <vector>

class Mesh : public QObject
{
    Q_OBJECT
public:
    explicit Mesh(QObject *parent = nullptr);

    void Initialize(const std::vector<GLfloat>& vertices, const std::vector<unsigned int>& indices, const std::vector<unsigned int>& borderIndices, unsigned int numOfVertices, unsigned int numOfIndices, unsigned int numOfBorderIndices);
    void Copy(Mesh* mesh);

    GLfloat* getVerticies();
    unsigned int* getIndices();
    unsigned int* getBorderIndices();
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    unsigned int getNumOfBorderIndices();
    QMatrix4x4 getModelMatrix();

    void UpdateGeometry(QVector3D hitPoint);

signals:

private:
    std::vector<GLfloat> m_verticies;
    std::vector<unsigned int> m_indices;
    std::vector<unsigned int> m_border_indices;
    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
    unsigned int m_numOfBorderIndices;
};

#endif // MESH_H
