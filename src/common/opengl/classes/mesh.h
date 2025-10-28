#ifndef MESH_H
#define MESH_H

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QMatrix4x4>
#include <QVector3D>

class Mesh : public QObject
{
    Q_OBJECT
public:
    explicit Mesh(QObject *parent = nullptr);

    void Initialize(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);

    GLfloat* getVerticies();
    unsigned int* getIndices();
    unsigned int getNumOfVertices();
    unsigned int getNumOfIndices();
    QMatrix4x4 getModelMatrix();

    void UpdateGeometry(QVector3D hitPoint);

signals:

private:
    GLfloat* m_verticies;
    unsigned int* m_indices;
    unsigned int m_numOfVertices;
    unsigned int m_numOfIndices;
};

#endif // MESH_H
