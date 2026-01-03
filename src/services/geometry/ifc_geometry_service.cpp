#include "ifc_geometry_service.h"
#include <vector>
#include <QFile>
//#include <QDataStream>
#include <QTextStream>

// using namespace FacetModeler;
// using namespace std;



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
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        Point point = referenceLine[i];

        // Vertex v = {
        //     {point[0], point[1], 0.0f},
        //     {0.0f, 0.0f, 1.0f},
        //     {0.0f, 0.0f},
        //     OpenGLMaterial::IVORY,
        //     Texture::NONE
        // };

        vertices_position.push_back({point[0], point[1], 0.0f, 0.0f});
        vertices_normal.push_back({0.0f, 0.0f, 1.0f});
        vertices_textureuv.push_back({0.0f, 0.0f});
        vertices_materialIndex.push_back(OpenGLMaterial::IVORY);
        vertices_textureIndex.push_back(Texture::NONE);

        // verticesVector.push_back(point[0]); // x
        // verticesVector.push_back(point[1]); // y
        // verticesVector.push_back(0.0f); // z
        // verticesVector.push_back(0.0f); // n.x
        // verticesVector.push_back(0.0f); // n.y
        // verticesVector.push_back(1.0f); // n.z
    }

    std::vector<EdgeIndex> edge_indices = {};
    std::vector<float> edge_width = {};
    std::vector<int> edge_materialIndex = {};
    for (int i = 0; i < referenceLine.size(); i++)
    {
        edge_width.push_back(1);
        edge_materialIndex.push_back(OpenGLMaterial::BLACK);

        if (i == referenceLine.size() - 1)
        {
            edge_indices.push_back({i, 0});
        }
        else
        {
            edge_indices.push_back({i, i + 1});
        }
    }

    // // Allocate memory for the new array using std::unique_ptr for safety.
    // auto indices_raw = std::make_unique<unsigned int[]>(indices.size());

    // // Copy elements from the vector to the new array.
    // std::copy(indices.begin(), indices.end(), indices_raw.get());

    // Mesh* mesh = new Mesh(this);
    mesh->Initialize(
        vertices_position,
        vertices_normal,
        vertices_textureuv,
        vertices_materialIndex,
        vertices_textureIndex,
        edge_indices,
        edge_width,
        edge_materialIndex,
        indices
        );
    mesh->setBIMElementId(ifcElement->getId());


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


void IfcGeometryService::generateMesh3D(OdIfcFilePtr ifcFilePtr, Mesh* mesh)
{
    OdIfcModelContext& modelContext = ifcFilePtr->getContext();

    //Choose what kinds of entities to compose (optional, but recommended)
    modelContext.getGeometryComposeTypes().append(OdIfc::kIfcProduct);

    //Set geometry options
    modelContext.setComposeOutOfSpatialStructure(true);
    modelContext.setDrawOpenings(true);
    modelContext.setDrawPoints(true);
    modelContext.setDrawSpaces(true);

    //Compose IFC geometry
    OdResult composeR =  ifcFilePtr->composeEntities();

    OdIfcModelPtr odIfcModelPtr = ifcFilePtr->getModel();

    const OdDAI::SetOfOdDAIObjectId* productIdSet = odIfcModelPtr->getEntityExtent("IfcProduct");
    if (!productIdSet || productIdSet->isNil())
    {
        qDebug() <<"No IfcProduct instances found in the model";
    }

    const OdDAIObjectIds& productIds = productIdSet->getArray();

    for (OdDAIObjectIds::size_type iProduct = 0; iProduct < productIds.size(); ++iProduct)
    {
        OdIfc::OdIfcInstancePtr pEntity = productIds[iProduct].openObject();
        if (pEntity.isNull())
        {
            continue;
        }

        OdIfc::OdIfcProductPtr pProduct = OdIfc::OdIfcInstance::asCompound(pEntity);
        if (pProduct.isNull())
        {
            continue;
        }

        /*
        const OdIfc::OdIfcGeometricRepresentationItemPtrArray geomItems = pProduct->getGeometricRepresentationItems();
        if(geomItems.length() > 0)
        {
            qInfo() << "pEntity Type: " << pEntity->isA()->name();
            qInfo() << "pProduct Type: " << pProduct->isA()->name();
            qInfo() << "Length of Gom items: " << geomItems.length();
        }
        */

        OdGeMatrix3d productTransformation = pProduct->getObjectPlacement();
        OdIfc::OdIfcRepresentationItemPtrArray repItems = pProduct->getRepresentationItems();

        for (auto repItem: repItems)
        {
            if (repItem->type() == OdIfc::kIfcMappedItem)
            {
                OdIfc::OdIfcMappedItemPtr mappedItemPtr = OdIfc::OdIfcMappedItem::cast(repItem);

                // qInfo() << "Mapped is null: " << mappedItemPtr.isNull();

                OdDAIObjectId mappingSource = mappedItemPtr->mappingSource();

                // qInfo() << "Mapping Source is null: " << mappingSource.isNull();

                const OdGeMatrix3d mappedTransformation = productTransformation * mappedItemPtr->mappingTarget();

                OdIfc::OdIfcInstancePtr pMappedEntity = mappingSource.openObject();

                // qInfo() << "IFC Instance is null: " << pMappedEntity.isNull();

                OdIfc2x3::IfcRepresentationMapPtr mapPtr = OdIfc2x3::IfcRepresentationMap::cast(pMappedEntity);

                //qInfo() << "Map Instance is null: " << mapPtr.isNull(); //pMappedEntity->isA()->name();
                // qInfo() << "Map Instance is null: " << pMappedEntity->isA()->name();

                OdDAIObjectId mappedRepresentation = mapPtr->getMappedRepresentation();

                // qInfo() << "Mapped Representation is null: " << mappedRepresentation.isNull();

                OdIfc::OdIfcInstancePtr pMappedRep = mappedRepresentation.openObject();
                // qInfo() << "Mapped Representation Pointer is null: " << pMappedRep.isNull();

                OdIfc2x3::IfcShapeRepresentationPtr shapePtr = OdIfc2x3::IfcShapeRepresentation::cast(pMappedRep);
                // qInfo() << "Shape is null: " << shapePtr.isNull();

                OdDAIObjectIds shapeItems;
                shapePtr->getItems(shapeItems);

                qInfo() << shapeItems.size();

                for (OdDAIObjectIds::size_type iItem = 0; iItem < shapeItems.size(); ++iItem)
                {
                    OdIfc::OdIfcInstancePtr pShapeEntity = shapeItems[iItem].openObject();                   
                    // qInfo() << "pShapeEntity is null: " << pShapeEntity->isA()->name();
                    try
                    {
                        OdIfc::OdIfcGeometricRepresentationItemPtr pShapeProduct = OdIfc::OdIfcGeometricRepresentationItem::cast(OdIfc::OdIfcInstance::asCompound(pShapeEntity));
                        // qInfo() << "pShapeProduct is null: " << pShapeProduct.isNull();
                        OdDAI::OdBodyVariant bodyContainer = pShapeProduct->bodyContainer();
                        generateFinalMesh3D(bodyContainer, mesh, mappedTransformation);
                    }
                    catch (const OdError& e)
                    {
                        // std::wcout << e.description().c_str() << std::endl;
                        //odPrintConsoleString(e.description());
                    }
                }
            }
            //else if(repItem->type() == OdIfc::kIfcExtrudedAreaSolid)
            else
            {
                qDebug() << "OdIfc:: Type::" << repItem->type();
                try
                {
                    OdIfc::OdIfcGeometricRepresentationItemPtr pGeomItem = OdIfc::OdIfcGeometricRepresentationItem::cast(repItem);
                    OdDAI::OdBodyVariant bodyContainer = pGeomItem->bodyContainer();
                    generateFinalMesh3D(bodyContainer, mesh, productTransformation);
                }
                catch (const OdError& e)
                {
                    //std::wcout << e.description().c_str() << std::endl;
                    //odPrintConsoleString(e.description());
                }
            }
        }
    }    
}

void IfcGeometryService::generateFinalMesh3D(OdDAI::OdBodyVariant bodyContainer, Mesh* mesh, const OdGeMatrix3d &transformationMatrix)
{
    std::vector<uint32_t> meshIndices = {};
    std::vector<Position> vertices_position = {};
    std::vector<Normal> vertices_normal = {};
    std::vector<TextureUV> vertices_textureuv = {};
    std::vector<int> vertices_materialIndex = {};
    std::vector<int> vertices_textureIndex = {};
    std::vector<EdgeIndex> edge_indices = {};
    std::vector<float> edge_width = {};
    std::vector<int> edge_materialIndex = {};
    int textureIndex = Texture::WOOD; // if less than zero then we don't need to worry about textures
    int materialIndex = OpenGLMaterial::IVORY;
    float edgeWidth = 1;
    int edgeMaterialIndex = OpenGLMaterial::BLACK;
    int scalingFactor = 5;

    switch (bodyContainer.kind())
    {
        case OdDAI::OdBodyVariant::kFacetModelerBody:
        {
            qDebug() << "FacetModelerBody";
            const FacetModeler::Body* fBody = bodyContainer.facetModelerBody();
            FacetModeler::Body clonedBody = fBody->clone();
            clonedBody.transform(transformationMatrix);

            m_openglHelper.getMeshGeometry(
                clonedBody,
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_width,
                edge_materialIndex,
                textureIndex,
                materialIndex,
                scalingFactor,
                edgeWidth,
                edgeMaterialIndex
            );
            break;
        }

        case OdDAI::OdBodyVariant::kMdBody:
        {
            qDebug() << "Md Body";

            m_openglHelper.getMeshGeometry(
                *bodyContainer.mdBody(),
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_width,
                edge_materialIndex,
                textureIndex,
                materialIndex,
                scalingFactor,
                edgeWidth,
                edgeMaterialIndex
            );
            break;
        }

        case OdDAI::OdBodyVariant::kAcisBody:
        {
            qDebug() << "Acis Body";

            m_openglHelper.getMeshGeometry(
                *bodyContainer.acisBody(),
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_width,
                edge_materialIndex,
                textureIndex,
                materialIndex,
                scalingFactor,
                edgeWidth,
                edgeMaterialIndex
            );
            break;
        }

        case OdDAI::OdBodyVariant::kBrep:
        {
            qDebug() << "IFC Brep Body";

            m_openglHelper.getMeshGeometry(
                bodyContainer.brBrep(),
                vertices_position,
                vertices_normal,
                vertices_textureuv,
                vertices_materialIndex,
                vertices_textureIndex,
                meshIndices,
                edge_indices,
                edge_width,
                edge_materialIndex,
                textureIndex,
                materialIndex,
                scalingFactor,
                edgeWidth,
                edgeMaterialIndex
            );
            break;
        }

        case OdDAI::OdBodyVariant::kEmpty:
        {
            qDebug() << "No Body";
            break;
        }
    }

    if (vertices_position.size() > 0 && meshIndices.size() > 0 && edge_indices.size() > 0) {
        mesh->AppendGeometry(
            vertices_position,
            vertices_normal,
            vertices_textureuv,
            vertices_materialIndex,
            vertices_textureIndex,
            edge_indices,
            edge_width,
            edge_materialIndex,
            meshIndices
        );

        // qInfo() << "Vertices size: " << verticesVector.size() << " Mesh Index: " << meshIndices.size() << " Border Index: " << borderIndices.size();
    }
}



