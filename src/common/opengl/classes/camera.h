#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLFunctions_3_3_Core>
#include <QtMath>

class Camera : public QObject, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    void Initialize(QVector3D startPosition, QVector3D startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);

    QVector3D getCameraPosition();
    QVector3D getCameraDirection();

    QMatrix4x4 calculateViewMatrix();

signals:

private:
    QVector3D position;
    QVector3D front;
    QVector3D up;
    QVector3D right;
    QVector3D worldUp;

    GLfloat yaw;
    GLfloat pitch;

    GLfloat moveSpeed;
    GLfloat turnSpeed;

    void update();
};

#endif // CAMERA_H
