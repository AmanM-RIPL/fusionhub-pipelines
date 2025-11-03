#ifndef WALL_GEOMETRY_SERVICE_H
#define WALL_GEOMETRY_SERVICE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QtMath>

#include <vector>
#include <array>
#include <cmath>
#include <limits>
#include <memory>
#include <algorithm>

#include "OdaCommon.h"
#include "RxObject.h"
#include "RxObjectImpl.h"
#include "ExSystemServices.h"
#include "StaticRxObject.h"
#include "FMMdlBody.h"
#include "FMMdlFace.h"
#include "FMMdlSurface.h"
#include "FMMdlEdge.h"
#include "FMMdlVertex.h"
#include "FMMdlIterators.h"
#include "FMContour2D.h"
#include "FMProfile2D.h"
#include "Ge/GeTrMeshSimplification.h"


#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"

using Point = std::array<float, 2>; // (x,y)
using Line = std::array<float, 3>; // (m, b, x) for y = mx + b and x in case m is infinity

class WallGeometryService : public QObject
{
    Q_OBJECT
public:
    explicit WallGeometryService(QObject *parent = nullptr);

    void generateMesh2D(BIMElement* wallElement, Mesh* mesh);
    void generateMesh3D(BIMElement *wallElement, Mesh* mesh);


signals:

private:
    void extractBIMParameters(BIMElement *wallElement, std::vector<Point>& referenceLine, float& width, float& height);

    std::vector<Point> generateParallelCurve(std::vector<Point> referenceCurve, float width);

    // [10,2] where 10 = m and 2 = b in y = mx + b
    Line getParallelLineEquation(Point point1, Point point2, float width);

    // [10, 2] where 10 is x and 2 is y
    Point getIntersectionPoint(Line line1, Line line2);

    // [10, 2] where 10 is x and 2 is y
    Point getParallelProjectionPoint(Point point1, Point point2, float width);
};

#endif // WALL_GEOMETRY_SERVICE_H
