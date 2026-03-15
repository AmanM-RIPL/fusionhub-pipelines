#ifndef WALL_H
#define WALL_H

#include <QObject>

#include "base_bim_model.h"
#include "common/opengl/classes/opengl_helper.h"

class Wall : public QObject, public BaseBimModel
{
    Q_OBJECT
public:
    explicit Wall(QObject *parent = nullptr);
    Wall(int id, QObject *parent = nullptr);

    std::vector<ReferenceLineSegment> referenceLine() const;
    void setReferenceLine(const std::vector<ReferenceLineSegment> &newReferenceLine);

    std::vector<Layer> layers() const;
    void setLayers(const std::vector<Layer> &newLayers);

    float width() const;
    void setWidth(float newWidth);

    float height() const;
    void setHeight(float newHeight);

    float distance() const;
    void setDistance(float newDistance);

    float slantAngle() const;
    void setSlantAngle(float newSlantAngle);

    float taperAngle() const;
    void setTaperAngle(float newTaperAngle);

    QString referenceLinePosition() const;
    void setReferenceLinePosition(const QString &newReferenceLinePosition);

private:
    std::vector<ReferenceLineSegment> m_referenceLine;
    std::vector<Layer> m_layers;
    float m_width = 0;
    float m_height = 0;
    float m_distance = 0;
    float m_slantAngle = 0;
    float m_taperAngle = 0;
    QString m_referenceLinePosition = "inner";

signals:
};

#endif // WALL_H
