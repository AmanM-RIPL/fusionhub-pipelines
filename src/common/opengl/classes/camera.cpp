#include "camera.h"

Camera::Camera(QObject *parent)
    : QObject{parent}
{}

void Camera::Initialize(QVector3D startTarget, QVector3D startUp, QVector3D startPosition, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    this->initializeOpenGLFunctions();

    SetCameraParameters(startTarget, startUp, startPosition, startMoveSpeed, startTurnSpeed);
}

void Camera::SetCameraParameters(QVector3D startTarget, QVector3D startUp, QVector3D startPosition, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    // initializing camera
    worldUp = startUp;
    position = startPosition;
    target = startTarget;
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    // initial calculations for pitch, and yaw
    QVector3D offset = position - target;
    distance = offset.length();

    yaw = qRadiansToDegrees(qAtan2(offset.z(), offset.x()));
    float horizontalDist = QVector2D(offset.x(), offset.y()).length();
    pitch = qRadiansToDegrees(qAtan2(offset.z(), horizontalDist));
    azimuthZ = qRadiansToDegrees(qAtan2(offset.y(), offset.x()));

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
        azimuthZ += turnSpeed;
    }
    else
    {
        azimuthZ -= turnSpeed;
    }

    update();
}

void Camera::Pan(float deltaX, float deltaY)
{
    QVector3D offset = (-right * deltaX * moveSpeed) + (up * deltaY * moveSpeed);
    position += offset;
    target += offset;

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

    // For the case of yaw and pitch
    // position.setX(target.x() + distance * qCos(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));
    // position.setY(target.y() + distance * qSin(qDegreesToRadians(pitch)));
    // position.setZ(target.z() + distance * qSin(qDegreesToRadians(yaw)) * qCos(qDegreesToRadians(pitch)));

    float az = qDegreesToRadians(azimuthZ);
    float el = qDegreesToRadians(pitch);

    // Distance projected onto XY plane
    float planarDist = distance * qCos(el);

    position.setX(target.x() + planarDist * qCos(az));
    position.setY(target.y() + planarDist * qSin(az));
    position.setZ(target.z() + distance * qSin(el));

    front = (target - position).normalized();
    cameraDirection = -front;

    right = QVector3D::crossProduct(worldUp, cameraDirection);
    right = right.normalized();


    up = QVector3D::crossProduct(cameraDirection, right);
    up = up.normalized();
}


