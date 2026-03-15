#include "wall.h"

Wall::Wall(QObject *parent)
    : QObject{parent}, BaseBimModel{-1, "Wall"}
{}

Wall::Wall(int id, QObject *parent)
    : QObject{parent}, BaseBimModel{id, "Wall"}
{}


std::vector<ReferenceLineSegment> Wall::referenceLine() const
{
    return m_referenceLine;
}

void Wall::setReferenceLine(const std::vector<ReferenceLineSegment> &newReferenceLine)
{
    m_referenceLine = newReferenceLine;
}

std::vector<Layer> Wall::layers() const
{
    return m_layers;
}

void Wall::setLayers(const std::vector<Layer> &newLayers)
{
    m_layers = newLayers;
}

float Wall::width() const
{
    return m_width;
}

void Wall::setWidth(float newWidth)
{
    m_width = newWidth;
}

float Wall::height() const
{
    return m_height;
}

void Wall::setHeight(float newHeight)
{
    m_height = newHeight;
}

float Wall::distance() const
{
    return m_distance;
}

void Wall::setDistance(float newDistance)
{
    m_distance = newDistance;
}

float Wall::slantAngle() const
{
    return m_slantAngle;
}

void Wall::setSlantAngle(float newSlantAngle)
{
    m_slantAngle = newSlantAngle;
}

float Wall::taperAngle() const
{
    return m_taperAngle;
}

void Wall::setTaperAngle(float newTaperAngle)
{
    m_taperAngle = newTaperAngle;
}

QString Wall::referenceLinePosition() const
{
    return m_referenceLinePosition;
}

void Wall::setReferenceLinePosition(const QString &newReferenceLinePosition)
{
    m_referenceLinePosition = newReferenceLinePosition;
}
