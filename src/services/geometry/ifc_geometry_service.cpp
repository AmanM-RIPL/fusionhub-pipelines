#include "ifc_geometry_service.h"
#include <vector>
using namespace FacetModeler;
using namespace std;

IfcGeometryService::IfcGeometryService(QObject *parent)
    : QObject{parent}
{}


void IfcGeometryService::generateMesh2D(BIMElement* ifcElement, Mesh* mesh)
{
    std::vector<std::vector<Point>> polygon;
    std::vector<Point> referenceLine = {};
    float width = 0;
    float height = 0;
    float distance = 0;

    m_openglHelper.extractBIMParameters(ifcElement, referenceLine, width, height, distance);

    // 3. Generate a parallel line
    std::vector<Point> parallelLine = m_openglHelper.generateParallelCurve(referenceLine, width);

    referenceLine.insert(referenceLine.end(), parallelLine.begin(), parallelLine.end());

    // for (Point point: referenceLine)
    // {
    //     qInfo() << "x: " << point[0] << " , y: " << point[1];
    // }

    // 4. Get triangulated mesh
    polygon.push_back(referenceLine);
    polygon.push_back({}); // for holes
    std::vector<uint32_t> indices = mapbox::earcut<uint32_t>(polygon);

    // for (uint32_t index: indices)
    // {
    //     qInfo() << index;
    // }

    // 5. Create and export mesh
    std::vector<GLfloat> verticesVector = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        Point point = referenceLine[i];
        verticesVector.push_back(point[0]); // x
        verticesVector.push_back(point[1]); // y
        verticesVector.push_back(0.0f); // z
        verticesVector.push_back(0.0f); // n.x
        verticesVector.push_back(0.0f); // n.y
        verticesVector.push_back(1.0f); // n.z
    }

    std::vector<uint32_t> borderIndices = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        borderIndices.push_back(i);

        if (i == referenceLine.size() - 1)
        {
            borderIndices.push_back(0);
        }
        else
        {
            borderIndices.push_back(i + 1);
        }
    }

    // // Allocate memory for the new array using std::unique_ptr for safety.
    // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // Copy elements from the vector to the new array.
    // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
    mesh->Initialize(verticesVector, indices, borderIndices, referenceLine.size() * 6, indices.size(), borderIndices.size());


    // GLfloat* vertices1 = mesh->getVerticies();
    // unsigned int* indices1 = mesh->getIndices();
    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << vertices1[6*i] << " , " << vertices1[6*i + 1] << " , " << vertices1[6*i + 2] << " , " << vertices1[6*i + 3] << " , " << vertices1[6*i + 4] << " , " << vertices1[6*i + 5];
    // }

    // qInfo() << "----------------------------------";

    // for (int i = 0; i < 6; i++)
    // {
    //     qInfo() << indices1[i];
    // }

    // return mesh;
}


void IfcGeometryService::generateMesh3D(OdIfcModelPtr pIfcModel, Mesh* mesh)
{
    //const OdDAIObjectIds &productIds = pIfcModel->getEntityExtent("IfcProduct")->getArray();
    const OdDAI::SetOfOdDAIObjectId* productIdSet = pIfcModel->getEntityExtent("IfcProduct");
    if (!productIdSet || productIdSet->isNil())
    {
        qDebug() <<"No IfcProduct instances found in the model";
    }

    const OdDAIObjectIds& productIds = productIdSet->getArray();

    for (const auto& productId : productIds)
    {
        OdIfc::OdIfcInstancePtr pEntity = productId.openObject();
        if (pEntity.isNull())
        {
            continue;
        }

        OdIfc::OdIfcProductPtr pProduct = OdIfc::OdIfcInstance::asCompound(pEntity);        
        if (pProduct.isNull() || pProduct->isKindOf(OdIfc::kIfcOpeningElement))
        {
            qDebug() << pEntity->typeName();
            continue;
        }

        const OdIfc::OdIfcGeometricRepresentationItemPtrArray geomItems = pProduct->getGeometricRepresentationItems();
        int x = geomItems.size();
        qDebug() <<"geomItemSize:"<< x;


        for (OdDAIObjectIds::size_type iItem = 0; iItem < geomItems.size(); ++iItem)
        {
            OdIfc::OdIfcGeometricRepresentationItemPtr pGeomItem = geomItems[iItem];
            OdDAI::OdBodyVariant bodyContainer = pGeomItem->bodyContainer();


            std::vector<uint32_t> meshIndices = {};
            std::vector<uint32_t> borderIndices = {};
            std::vector<GLfloat> verticesVector = {};

            switch (bodyContainer.kind())
            {
            case OdDAI::OdBodyVariant::kFacetModelerBody:
            {
                qDebug() << "FacetModelerBody";              
                m_openglHelper.getMeshGeometry(*bodyContainer.facetModelerBody(), verticesVector, meshIndices, borderIndices);
                break;
            }

            case OdDAI::OdBodyVariant::kMdBody:
            {
                qDebug() << "Md Body";
                m_openglHelper.getMeshGeometry(*bodyContainer.mdBody(), verticesVector, meshIndices, borderIndices);
                break;
            }

            case OdDAI::OdBodyVariant::kAcisBody:
            {
                qDebug() << "Acis Body";
                m_openglHelper.getMeshGeometry(*bodyContainer.acisBody(), verticesVector, meshIndices, borderIndices);
                break;
            }

            case OdDAI::OdBodyVariant::kBrep:
            {
                qDebug() << "IFC Brep Body";               
                m_openglHelper.getMeshGeometry(bodyContainer.brBrep(), verticesVector, meshIndices, borderIndices);
                break;
            }

            case OdDAI::OdBodyVariant::kEmpty:
            {
                qDebug() << "No Body";
                break;
            }                
            }            

            mesh->Initialize(verticesVector, meshIndices, borderIndices, verticesVector.size(), meshIndices.size(), borderIndices.size());
        }
    }
}

