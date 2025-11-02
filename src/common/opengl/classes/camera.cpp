#include "camera.h"

Camera::Camera(QObject *parent)
    : QObject{parent}
{}

void Camera::Initialize(QVector3D startTarget, QVector3D startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startDistance, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    this->initializeOpenGLFunctions();

    // initializing camera
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    target = startTarget;
    distance = startDistance;
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

void Camera::Zoom(bool zoomIn)
{
    // Adjust distance instead of moving along front
    if (zoomIn)
        distance -= moveSpeed;
    else
        distance += moveSpeed;

    // Clamp to avoid flipping through target
    if (distance < 0.5f)
        distance = 0.5f;
    if (distance > 100.0f)
        distance = 100.0f;

    update();
}

void Camera::OrbitVertical(bool zDirection)
{
    if (zDirection)
    {
        pitch += turnSpeed;
    }
    else
    {
        pitch -= turnSpeed;
    }


    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }


    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

void Camera::OrbitHorizontal(bool rightDirection)
{
    if (rightDirection)
    {
        yaw += turnSpeed;
    }
    else
    {
        yaw -= turnSpeed;
    }

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
        target,
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

    // front.setX(qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    // front.setY(qSin(qDegreesToRadians(pitch)));
    // front.setZ(qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    // front = front.normalized();

    position.setX(target.x() + distance * qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    position.setY(target.y() + distance * qSin(qDegreesToRadians(pitch)));
    position.setZ(target.z() + distance * qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));

    front = (target - position).normalized();

    right = QVector3D::crossProduct(front, worldUp);
    right = right.normalized();


    up = QVector3D::crossProduct(right, front);
    up = up.normalized();
}


