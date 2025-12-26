#include "opengl_helper.h"

#include "models/bim_element.h"
#include "common/opengl/classes/mesh.h"
#include "common/opengl/classes/earcut_algorithm.h"

#include "IfcCore.h"
#include "IfcModel.h"
#include "IfcProduct.h"
//#include "FacetModeler/Body.h"
//#include "OdIfcRoot.h"


#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"

#include "common/myglitem.h"
#include "common/ifcdetail.h"

#include "database/database_manager.h"

#include "Br/BrBrep.h"
#include "BrepRenderer/BrepRendererImpl.h"
#include "Br/BrFace.h"
#include "Br/BrTraverser.h"

#include "Modeler/FMMdlBody.h"
#include "Modeler/FMDrawBody.h"
#include "Ge/GeCircArc2d.h"
#include <vector>

#include <QFile>
//#include <QDataStream>
#include <QTextStream>

// using namespace FacetModeler;
// using namespace std;

OpenglHelper::OpenglHelper(QObject *parent)
    : QObject{parent}
{}

void OpenglHelper::extractBIMParameters(BIMElement *wallElement, std::vector<Point> &referenceLine, float &width, float &height, float& distance)
{
    QList<BIMParameter*> parameterList = wallElement->getParameterList();

    // 1. Find Reference Line parameter and convert to a list of list (2D)

    // 2. Find Width
    QString widthString = "0";
    QString heightString = "0";
    QString distanceString = "0";
    QString referenceLineString = "[]";

    for (BIMParameter* parameter: parameterList)
    {
        if (parameter->getKey() == "ReferenceLine")
        {
            referenceLineString = parameter->getValue();
        }
        else if (parameter->getKey() == "Width")
        {
            widthString = parameter->getValue();
        }

        else if (parameter->getKey() == "Height")
        {
            heightString = parameter->getValue();
        }

        else if (parameter->getKey() == "Distance")
        {
            distanceString = parameter->getValue();
        }
    }

    bool ok;
    width = widthString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }


    height = heightString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    distance = distanceString.toFloat(&ok);
    if (!ok)
    {
        // ignore for now
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(referenceLineString.toUtf8());
    QJsonArray jsonArray = jsonDoc.array();

    for (const QJsonValue& outerValue : jsonArray)
    {
        QJsonArray innerArray = outerValue.toArray();
        Point innerList = {innerArray.at(0).toDouble(), innerArray.at(1).toDouble()};
        referenceLine.push_back(innerList);
    }

   // height = 4;
}

std::vector<Point> OpenglHelper::generateParallelCurve(std::vector<Point> referenceCurve, float width)
{
    std::vector<Point> parallelCurve;

    // Get Line Equation for first 2 points
    Point firstPoint = getParallelProjectionPoint(referenceCurve[0], referenceCurve[1], width);
    parallelCurve.insert(parallelCurve.begin(), firstPoint);

    // Get intersection points for the middle points
    if (referenceCurve.size() > 2)
    {
        for (int i = 1; i < referenceCurve.size() - 1; i++)
        {
            Line line1 = getParallelLineEquation(referenceCurve[i - 1], referenceCurve[i], width);
            Line line2 = getParallelLineEquation(referenceCurve[i], referenceCurve[i + 1], width);

            // in case both line have slope of infinity then we just parallely project it
            if (std::isinf(line1[0]) && std::isinf(line2[0]))
            {
                Point intersectionPoint = getParallelProjectionPoint(referenceCurve[i], referenceCurve[i + 1], width);
                parallelCurve.insert(parallelCurve.begin(), intersectionPoint);
            }
            else
            {
                Point intersectionPoint = getIntersectionPoint(line1, line2);
                parallelCurve.insert(parallelCurve.begin(), intersectionPoint);
            }
        }
    }

    // Get last point
    Point lastPoint = getParallelProjectionPoint(referenceCurve.back(), referenceCurve[referenceCurve.size() - 2], width);
    parallelCurve.insert(parallelCurve.begin(), lastPoint);

    return parallelCurve;
}

Line OpenglHelper::getParallelLineEquation(Point point1, Point point2, float width)
{
    // when slope is not infinite
    if (point2[0] - point1[0] != 0)
    {
        float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
        float b = point1[1] - point1[0] * m;
        float b_parallel = b + (width * qSqrt(qPow(m,2) + 1));

        Line result = {m, b_parallel, 0};

        return result;
    }

    Line result = {std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), point1[0] - width};
    return result;
}

Point OpenglHelper::getIntersectionPoint(Line line1, Line line2)
{
    // both line1 and line2 cannot have m infinity
    // if they do then use the getParallelProjectionPoint logic
    if (std::isinf(line1[0]))
    {
        float x = line1[2];
        float y = line2[0] * x + line2[1];

        Point result = {x, y};
        return result;
    }
    else if (std::isinf(line2[0]))
    {
        float x = line2[2];
        float y = line1[0] * x + line1[1];

        Point result = {x, y};
        return result;
    }


    float x = (line2[1] - line1[1]) / (line1[0] - line2[0]);
    float y = line1[0] * x + line1[1];

    Point result = {x, y};

    return result;
}

Point OpenglHelper::getParallelProjectionPoint(Point point1, Point point2, float width)
{
    // m = (y2 - y1)/(x2-x1) if x2 = x1 then the slope is infinite.
    // so we handle that case seperately

    if (point2[0] - point1[0] != 0)
    {
        float m = (point2[1] - point1[1]) / (point2[0] - point1[0]);
        float x_proj = point1[0] + width * (-m/qSqrt(qPow(m,2) + 1));
        float y_proj = point1[1] + width * (1/qSqrt(qPow(m,2) + 1));

        Point result = {x_proj, y_proj};

        return result;
    }

    Point result = {point1[0] - width, point1[1]};
    return result;
}

void OpenglHelper::getMeshGeometry(const FacetModeler::Body& body, std::vector<Vertex>& verticesVector, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, int textureIndex, int scalingFactor)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    if (textureIndex < 0)
    {
        FacetModeler::Face* face = body.faceList();
        for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
        {
            OdGePoint3dArray facePointArray = {};
            std::vector<int> numOfEdgesInLoop = {};
            std::vector<std::vector<Point>> earcutPolygon = {};

            // Face Plane Coordinate System
            OdGePoint3d origin;
            OdGeVector3d axis1;
            OdGeVector3d axis2; // this is in the plane not normal to it.
            OdGeVector3d normal = face->normal();

            face->plane().getCoordSystem(origin, axis1, axis2);

            for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
            {
                std::vector<Point> loopPolygon = {};
                numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

                FacetModeler::Edge* edge = face->edge(loopIndex);
                for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
                {
                    facePointArray.append(edge->startPoint());

                    unsigned int startPointIndex = 0;
                    bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                    if (!startResult)
                    {
                        pointArray.append(edge->startPoint());
                        normalArray.append(normal);
                        textureArray.push_back({0.0f, 0.0f});

                        startPointIndex = pointArray.size() - 1;
                    }


                    unsigned int endPointIndex = 0;
                    bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                    if (!endResult)
                    {
                        pointArray.append(edge->endPoint());
                        normalArray.append(normal);
                        textureArray.push_back({0.0f, 0.0f});

                        endPointIndex = pointArray.size() - 1;
                    }

                    borderIndices.push_back(startPointIndex);
                    borderIndices.push_back(endPointIndex);

                    // get two dimensional point on the Face Plane
                    OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                    float x = vecOnPlane.dotProduct(axis1);
                    float y = vecOnPlane.dotProduct(axis2);
                    loopPolygon.push_back({x,y});

                    edge = edge->next();
                }

                earcutPolygon.push_back(loopPolygon);
            }

            // generate the 2D tessellations
            std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

            // project index from indices to pointArray
            for (uint32_t index: indices)
            {
                OdGePoint3d pointInFacePointArray = facePointArray[index];

                unsigned int selectedIndex = 0;
                bool result = pointArray.find(pointInFacePointArray, selectedIndex);

                meshIndices.push_back(selectedIndex);
                normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
                normalArray[selectedIndex].normalize();
            }

            face = face->next();
        }
    }
    else
    {
        FacetModeler::Face* face = body.faceList();
        for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
        {
            OdGePoint3dArray facePointArray = {};
            std::vector<std::array<float, 2>> faceTextureArray = {};
            std::vector<uint32_t> faceBorderIndices = {};
            std::vector<std::vector<Point>> earcutPolygon = {};

            // Face Plane Coordinate System
            OdGePoint3d origin;
            OdGeVector3d axis1;
            OdGeVector3d axis2; // this is in the plane not normal to it.
            OdGeVector3d normal = face->normal();

            face->plane().getCoordSystem(origin, axis1, axis2);

            for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
            {
                std::vector<Point> loopPolygon = {};

                FacetModeler::Edge* edge = face->edge(loopIndex);
                int startingFacePointIndex = facePointArray.size();
                for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
                {
                    facePointArray.append(edge->startPoint());
                    faceBorderIndices.push_back(facePointArray.size() - 1); // for first point

                    if (edgeIndex < face->loopEdgeCount(loopIndex) - 1)
                    {
                        faceBorderIndices.push_back(facePointArray.size()); // for second point
                    }
                    else
                    {
                        faceBorderIndices.push_back(startingFacePointIndex); // for last point the end point is first point
                    }

                    // get two dimensional point on the Face Plane
                    OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                    float x = vecOnPlane.dotProduct(axis1);
                    float y = vecOnPlane.dotProduct(axis2);
                    loopPolygon.push_back({x,y});
                    faceTextureArray.push_back({x/scalingFactor, y/scalingFactor});

                    edge = edge->next();
                }

                earcutPolygon.push_back(loopPolygon);
            }

            // generate the 2D tessellations
            std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

            // project index from indices to pointArray
            int startingPointArrayIndex = pointArray.size();
            for (uint32_t index: indices)
            {
                meshIndices.push_back(startingPointArrayIndex + index);
            }

            for (int faceBorderIndex: faceBorderIndices)
            {
                borderIndices.push_back(startingPointArrayIndex + faceBorderIndex);
            }

            // add face points to pointArray
            for (int i = 0; i < facePointArray.size(); i++)
            {
                OdGePoint3d pointInFacePointArray = facePointArray[i];
                std::array<float, 2> textureUV = faceTextureArray[i];

                pointArray.append(pointInFacePointArray);
                normalArray.append(normal);
                textureArray.push_back(textureUV);
            }

            face = face->next();
        }

    }


    // Create local mesh
    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        Vertex v = {
            {point.x, point.y, point.z},
            {normal.x, normal.y, normal.z},
            {textureUV[0], textureUV[1]},
            OpenGLMaterial::IVORY,
            textureIndex
        };

        verticesVector.push_back(v);

        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }


    //File Writitng start
    // QString filePath = "D://Meshfile_1.txt";
    // QFile file(filePath);
    // if (file.open(QFile::WriteOnly | QFile::Append | QFile::Text)) {
    //     QTextStream out(&file);
    //     for (int i = 0; i < pointArray.size(); i++)
    //     {
    //         OdGePoint3d point = pointArray[i];
    //         out<<point.x;
    //         out<<", ";

    //         out<<point.y;
    //         out<<", ";

    //         out<<point.z;
    //         out<<"\n";
    //     }
    //     file.close();
    // }
    // else
    // {
    //     qDebug() << "Could not open file for writing:" << file.errorString();
    // }
    //End of file writting
}

void OpenglHelper::getMeshGeometry(const OdMdBody& body, std::vector<Vertex>& verticesVector, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, int textureIndex, int scalingFactor)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    //FacetModeler::FaceIterator itf(body);
    //itf.get()->normal();
    /*
    FacetModeler::Face* face = body.faceList();
    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
   */

    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        Vertex v = {
            {point.x, point.y, point.z},
            {normal.x, normal.y, normal.z},
            {textureUV[0], textureUV[1]},
            0,
            textureIndex
        };

        verticesVector.push_back(v);
        // OdGePoint3d point = pointArray[i];
        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // OdGeVector3d normal = normalArray[i];
        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }
}

void OpenglHelper::getMeshGeometry(const BODY& body, std::vector<Vertex>& verticesVector, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, int textureIndex, int scalingFactor)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    //FacetModeler::FaceIterator itf(body);
    //itf.get()->normal();

    /*
    FacetModeler::Face* face = body.faceList();
    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
    */

    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        Vertex v = {
            {point.x, point.y, point.z},
            {normal.x, normal.y, normal.z},
            {textureUV[0], textureUV[1]},
            0,
            textureIndex
        };

        verticesVector.push_back(v);
        // OdGePoint3d point = pointArray[i];
        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // OdGeVector3d normal = normalArray[i];
        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }
}

void OpenglHelper::getMeshGeometry(const OdBrBrep& brep, std::vector<Vertex>& verticesVector, std::vector<uint32_t>& meshIndices, std::vector<uint32_t>& borderIndices, int textureIndex, int scalingFactor)
{
    OdGePoint3dArray pointArray = {};
    OdGeVector3dArray normalArray = {};
    std::vector<std::array<float, 2>> textureArray = {};

    /*
    FacetModeler::Face* face = body.faceList();

    for (int faceIndex = 0; faceIndex < body.faceCount(); faceIndex++)
    {
        OdGePoint3dArray facePointArray = {};
        std::vector<int> numOfEdgesInLoop = {};
        std::vector<std::vector<Point>> earcutPolygon = {};

        //Face Plane Coordinate System
        OdGePoint3d origin;
        OdGeVector3d axis1;
        OdGeVector3d axis2; // this is in the plane not normal to it.
        OdGeVector3d normal = face->normal();

        face->plane().getCoordSystem(origin, axis1, axis2);

        for (int loopIndex = 0; loopIndex < face->loopCount(); loopIndex++)
        {
            std::vector<Point> loopPolygon = {};
            numOfEdgesInLoop.push_back(face->loopEdgeCount(loopIndex));

            FacetModeler::Edge* edge = face->edge(loopIndex);
            for (int edgeIndex = 0; edgeIndex < face->loopEdgeCount(loopIndex); edgeIndex++)
            {
                facePointArray.append(edge->startPoint());

                unsigned int startPointIndex = 0;
                bool startResult = pointArray.find(edge->startPoint(), startPointIndex);
                if (!startResult)
                {
                    pointArray.append(edge->startPoint());
                    normalArray.append(normal);

                    startPointIndex = pointArray.size() - 1;
                }


                unsigned int endPointIndex = 0;
                bool endResult = pointArray.find(edge->endPoint(), endPointIndex);
                if (!endResult)
                {
                    pointArray.append(edge->endPoint());
                    normalArray.append(normal);

                    endPointIndex = pointArray.size() - 1;
                }

                borderIndices.push_back(startPointIndex);
                borderIndices.push_back(endPointIndex);

                // get two dimensional point on the Face Plane
                OdGeVector3d vecOnPlane = edge->startPoint() - origin;
                float x = vecOnPlane.dotProduct(axis1);
                float y = vecOnPlane.dotProduct(axis2);
                loopPolygon.push_back({x,y});

                edge = edge->next();
            }

            earcutPolygon.push_back(loopPolygon);
        }

        // generate the 2D tessellations
        std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(earcutPolygon);

        // project index from indices to pointArray
        for (uint32_t index: indices)
        {
            OdGePoint3d pointInFacePointArray = facePointArray[index];

            unsigned int selectedIndex = 0;
            bool result = pointArray.find(pointInFacePointArray, selectedIndex);

            meshIndices.push_back(selectedIndex);
            normalArray[selectedIndex] = normalArray[selectedIndex] + normal;
            normalArray[selectedIndex].normalize();
        }

        face = face->next();
    }
*/

    for (int i = 0; i < pointArray.size(); i++)
    {
        OdGePoint3d point = pointArray[i];
        OdGeVector3d normal = normalArray[i];
        std::array<float, 2> textureUV = textureArray[i];

        Vertex v = {
            {point.x, point.y, point.z},
            {normal.x, normal.y, normal.z},
            {textureUV[0], textureUV[1]},
            0,
            textureIndex
        };

        verticesVector.push_back(v);
        // OdGePoint3d point = pointArray[i];
        // verticesVector.push_back(point.x); // x
        // verticesVector.push_back(point.y); // y
        // verticesVector.push_back(point.z); // z

        // OdGeVector3d normal = normalArray[i];
        // verticesVector.push_back(normal.x); // n.x
        // verticesVector.push_back(normal.y); // n.y
        // verticesVector.push_back(normal.z); // n.z
    }
}

