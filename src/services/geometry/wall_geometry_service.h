#ifndef WALL_GEOMETRY_SERVICE_H
#define WALL_GEOMETRY_SERVICE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QtMath>

#include <vector>
#include <array>

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"

using Point = std::array<float, 2>;
using Line = std::array<float, 2>;

class WallGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WallGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement);


signals:

private:
    std::vector<Point> generateParallelCurve(std::vector<Point> referenceCurve, float width);

    // [10,2] where 10 = m and 2 = b in y = mx + b
    Line getParallelLineEquation(Point point1, Point point2, float width);

    // [10, 2] where 10 is x and 2 is y
    Point getIntersectionPoint(Line line1, Line line2);

    // [10, 2] where 10 is x and 2 is y
    Point getParallelProjectionPoint(Point point1, Point point2, float width);
};

#endif // WALL_GEOMETRY_SERVICE_H
