#include "camera.h"

Camera::Camera(QObject *parent)
    : QObject{parent}
{}

void Camera::Initialize(QVector3D startPosition, QVector3D startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    this->initializeOpenGLFunctions();

    // initializing camera
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = QVector3D(0.0f, 0.0f, -1.0f);
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

QVector3D Camera::getCameraPosition()
{
    return position;
}

QVector3D Camera::getCameraDirection()
{
    return front.normalized();
}

QMatrix4x4 Camera::calculateViewMatrix()
{
    QMatrix4x4 view;
    view.setToIdentity();

    view.lookAt(
        position,
        position + front,
        up
    );

    return view;
}

void Camera::update()
{
    /*
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    */

    front.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front.setY(qSin(qDegreesToRadians(pitch)));
    front.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    front = front.normalized();

    right = QVector3D::crossProduct(front, worldUp);
    right = right.normalized();


    up = QVector3D::crossProduct(right, front);
    up = up.normalized();
}


