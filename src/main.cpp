#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCoreApplication>
#include <QDebug>
#include <QSGRendererInterface>
#include <random>
#include <QDir>

/*
#include "OdaCommon.h"
#include "OdString.h"
#include "daiObjectId.h"
#include "OdArray.h"
#include "Ge/GeVector3dArray.h"
*/

// #include <iostream>

 #ifndef SPA_NO_AUTO_LINK
 #define SPA_NO_AUTO_LINK
 #endif

#define M_PI 3.14159265358979323846

#include "common/myglitem.h"
#include "common/ifcdetail.h"

#include "database/database_manager.h"

#include "repositories/user_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/budget_head_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/material_repository.h"
#include "repositories/schedule_setup_repository.h"
#include "repositories/schedule_of_rates_repository.h"
#include "repositories/ifc_detail_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/bim_element_repository.h"
#include "repositories/bill_of_quantity_repository.h"
#include "repositories/task_repository.h"
#include "repositories/work_order_repository.h"
#include "repositories/work_order_line_repository.h"
#include "repositories/work_billing_repository.h"
#include "repositories/work_billing_line_repository.h"
#include "repositories/file_repository.h"
#include "repositories/purchase_order_repository.h"
#include "repositories/purchase_order_line_repository.h"
#include "repositories/good_received_note_repository.h"

#include "controllers/user_controller.h"
#include "controllers/project_controller.h"
#include "controllers/budget_head_controller.h"
#include "controllers/vendor_controller.h"
#include "controllers/material_controller.h"
#include "controllers/unit_of_measurement_controller.h"
#include "controllers/schedule_setup_controller.h"
#include "controllers/schedule_of_rates_controller.h"
#include "controllers/ifc_detail_controller.h"
#include "controllers/ifc_controllers/ifc_wall_controller.h"
#include "controllers/bim_element_controller.h"
#include "controllers/bill_of_quantity_controller.h"
#include "controllers/bill_of_quantity_line_controller.h"
#include "controllers/task_controller.h"
#include "controllers/work_order_controller.h"
#include "controllers/work_billing_controller.h"
#include "controllers/work_billing_line_controller.h"
#include "controllers/work_order_line_controller.h"
#include "controllers/project_budget_controller.h"
#include "controllers/file_controller.h"
#include "controllers/purchase_order_controller.h"
#include "controllers/purchase_order_line_controller.h"
#include "controllers/good_received_note_controller.h"
#include "controllers/material_indent_controller.h"
#include "controllers/draft_entity_controller.h"

#include "models/user.h"
#include "models/unit_of_measurement.h"
#include "models/draft_entity.h"
#include "common/helper_point.h"

#include "network/network_manager.h"

#include <iostream>


#include "acis.hxx"
#include "license.hxx"
#include "spa_unlock_result.hxx"
#include "spatial_license.h"
#include "kernapi.hxx"

#include <boolapi.hxx>
#include "api.hxx"
#include "lists.hxx"
#include "fileinfo.hxx"
#include <cstrapi.hxx>
#include "curdef.hxx"
#include "straight.hxx"
#include "sweepapi.hxx"
#include "swp_opts.hxx"

// for faceter
#include "af_api.hxx"
#include "fct_utl.hxx"
#include "af_serializable_mesh.hxx"

const char* unlock_str = SPATIAL_LICENSE;

// There has to be a better way???????????
const OdString OdString::kEmpty;
const OdDAIObjectId OdDAIObjectId::kNull;
OdArrayBuffer OdArrayBuffer::g_empty_array_buffer;
OdGeTol OdGeContext::gTol;
FacetModeler::DeviationParams FacetModeler::FMGeGbl::gDefDev;
const OdGePoint3d OdGePoint3d::kOrigin;
OdGeTol FacetModeler::FMGeGbl::gTol;

//Create global state later
std::shared_ptr<User> gUser = std::make_shared<User>();
int gTenantId = 0;
int gProjectId = 0;
QString gProjectName ="";
QString gEnvironmentPath = "C:\\Users\\RIPL\\Documents\\FusionHubData";
OdStaticRxObject<MyServices> svcs;


void unlock_spatial_products()
{
   spa_unlock_result res = spa_unlock_products(unlock_str);

   qDebug() << "SPATIAL License info:" << res.get_message_text();
}

int main(int argc, char *argv[])
{
    //static OdStaticRxObject<MyServices> svcs;

    // api_start_modeller(0);
    // unlock_spatial_products();

    // ENTITY_LIST ents;

    // // Create the first solid block
    // // BODY* block = nullptr;
    // // api_solid_block(SPAposition(0, 0, 0), SPAposition(10, 10, 10), block);
    // // ents.add(block);
    // // qInfo() << "Created Block: " << block->size();

    // // Create wire body
    // BODY* wire_body = nullptr;

    // EDGE* edge1 = nullptr;
    // EDGE* edge2 = nullptr;
    // EDGE* edge3 = nullptr;
    // EDGE* edge4 = nullptr;

    // api_curve_line(SPAposition(0,0,0), SPAposition(1,0,0), edge1);
    // // api_curve_arc_3pt(SPAposition(0,0,0), SPAposition(0.5, -0.5, 0), SPAposition(1,0,0), false, edge1);
    // api_curve_line(SPAposition(1,0,0), SPAposition(1,1,0), edge2);
    // api_curve_line(SPAposition(1,1,0), SPAposition(0,1,0), edge3);
    // api_curve_line(SPAposition(0,1,0), SPAposition(0,0,0), edge4);

    // std::vector<EDGE*> edges = { edge1, edge2, edge3, edge4 };

    // api_make_ewire(4, edges.data(), wire_body);

    // // sweep
    // BODY* new_body = nullptr;

    // EXCEPTION_BEGIN
    //     sweep_options* sw_options = ACIS_NEW sweep_options();
    // // sw_options->set_draft_angle(M_PI * 0.1);
    // EXCEPTION_TRY
    //     outcome result_outcome = api_sweep_with_options(wire_body, SPAvector(0,0,1), sw_options, new_body);

    //     // error_info* error_result = result_outcome.get_error_info();
    //     // qInfo() << "result_outcome: " << error_result->error_message();

    // EXCEPTION_CATCH_TRUE
    //     ACIS_DELETE sw_options;
    // EXCEPTION_END

    // // facet code
    // api_facet_entity(wire_body);

    // ENTITY_LIST faces;
    // api_get_faces(wire_body, faces);

    // faces.init();
    // for (int i = 0; i < faces.iteration_count(); i++)
    // {
    //     ENTITY* itr = faces.next();

    //     qInfo() << "Face is there";

    //     std::vector<float> coords;
    //     std::vector<int> triangles;
    //     std::vector<float> normal_coords;
    //     std::vector<float> uv_coords;

    //     af_serializable_mesh* sm = GetSerializableMesh((FACE*)itr);
    //     if (sm == NULL)
    //     {
    //         continue;
    //     }

    //     const int nv = sm->number_of_vertices();
    //     int ntri = sm->number_of_polygons();

    //     coords.resize(3 * nv);
    //     sm->serialize_positions(coords.data());

    //     bool const has_normals = sm->has_normals() == TRUE;
    //     if (has_normals)
    //     {
    //         normal_coords.resize(3 * nv);
    //     }
    //     sm->serialize_normals(normal_coords.data());

    //     triangles.resize(3 * ntri);
    //     int ntri_actual = sm->serialize_triangles(triangles.data());
    //     while (ntri_actual < ntri)
    //     {
    //         triangles.pop_back();
    //         ntri_actual = static_cast<int>(triangles.size());
    //     }

    //     bool const has_uvs = sm->has_uv() == TRUE;
    //     if (has_uvs)
    //     {
    //         uv_coords.resize(2 * nv);
    //     }
    //     sm->serialize_uv_data(uv_coords.data(), true);

    //     for (int i = 0; i < coords.size(); i = i + 3)
    //     {
    //         qInfo() << "Coords: (" << coords[i] << ", " << coords[i + 1] << ", " << coords[i + 2] << ")";
    //     }

    //     for (int i = 0; i < normal_coords.size(); i = i + 3)
    //     {
    //         qInfo() << "Normals: (" << normal_coords[i] << ", " << normal_coords[i + 1] << ", " << normal_coords[i + 2] << ")";
    //     }

    //     for (int i = 0; i < uv_coords.size(); i = i + 2)
    //     {
    //         qInfo() << "UVs: (" << uv_coords[i] << ", " << uv_coords[i + 1] << ")";
    //     }

    //     for (int i = 0; i < triangles.size(); i = i + 3)
    //     {
    //         qInfo() << "Triangles: (" << triangles[i] << ", " << triangles[i + 1] << ", " << triangles[i + 2] << ")";
    //     }

    //     // calculate edge data
    //     FACE* face_itr = (FACE*)itr;
    //     LOOP* loop = face_itr->loop();
    //     const LOOP* first_loop = loop;

    //     do
    //     {
    //         qInfo() << "New Loop";
    //         // get co-edges in loop
    //         COEDGE* coedge = loop->start();
    //         COEDGE* first_coedge = coedge;

    //         do
    //         {
    //             qInfo() << "New Edge";
    //             EDGE* edge = coedge->edge();

    //             SPAposition* pos_array = nullptr;
    //             int numOfEdgeVertices = 0;
    //             api_get_facet_edge_points(edge, pos_array, numOfEdgeVertices);

    //             for (int i = 0; i < numOfEdgeVertices; i++)
    //             {
    //                 SPAposition& pos = pos_array[i];
    //                 qInfo() << "Position of edge: " << pos.x() << ", " << pos.y() << ", " << pos.z();
    //             }

    //             coedge = coedge->next();
    //         }
    //         while (coedge != first_coedge);

    //         loop = loop->next();
    //     }
    //     while ((loop != first_loop) && (loop != nullptr));
    // }

    odrxInitialize(&svcs);

    odIfcInitialize(true, true);
    odTvInitialize();

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "NetworkManager",
        NetworkManager::getInstance()
        );

    QDir dir;
    if (!dir.exists(gEnvironmentPath)) {
        if (dir.mkpath(gEnvironmentPath)) {
            qDebug() << "Directory" << gEnvironmentPath << "created successfully.";
        } else {
            qDebug() << "Failed to create directory" << gEnvironmentPath;
        }
    } else {
        qDebug() << "Directory" << gEnvironmentPath << "already exists.";
    }

    /*
        HERE WE EXECUTE THE IFC CODE

    */
     //QList<IFCDetail*> ifcDetailList;
    // OdTvFactoryId factId = odTvGetFactory();

    // OdTvResult rc;
    // OdTvDatabaseId dbId = factId.createDatabase(&rc);
    // OdTvDatabasePtr pTvDatabase = dbId.openObject(OdTv::kForWrite, &rc);

    // OdTvModelId modelId = pTvDatabase->createModel(OD_T("Model1"));
    // OdTvModelPtr modelPtr = modelId.openObject(OdTv::kForWrite, &rc);

    // MyGLItem::setDatabaseId(dbId);
    // MyGLItem::setModelId(modelId);


    // Read IFC File
    // OdString ifcFileName("C:\\Users\\RIPL\\Downloads\\DblDoor-1-Panel.ifc"); // BasicHouse

    // OdIfcFilePtr pDatabase;

    // pDatabase = svcs.createDatabase();
    // OdResult res = pDatabase->readFile(ifcFileName);
    // qInfo() << "Database Loaded: " << (res == tvOk);

    // OdIfcModelPtr pIfcModel = pDatabase->getModel();

    // OdIfcModelContext& modelContext = pDatabase->getContext();

    // // Choose what kinds of entities to compose (optional, but recommended)
    // modelContext.getGeometryComposeTypes().append(OdIfc::kIfcProduct);

    // // Set geometry options
    // modelContext.setComposeOutOfSpatialStructure(true);
    // modelContext.setDrawOpenings(true);
    // modelContext.setDrawPoints(true);
    // modelContext.setDrawSpaces(true);

    // OdArray<OdIfc::OdIfcEntityType> geomTypes = {OdIfc::kIfcDoor};
    // modelContext.setGeometryComposeTypes(geomTypes);

    // Compose IFC geometry
    // OdResult composeR =  pDatabase->composeEntities();

    // // get CDA data
    // TreeItem* rootItem = new TreeItem("", "", "");

    // CDAWalker walker(new CDATreePrinter);
    // walker.run(pDatabase, rootItem);

    //TreeModel* treeModel = new TreeModel(rootItem, nullptr);


    // const OdDAI::SetOfOdDAIObjectId* productIdSet = pIfcModel->getEntityExtent("IfcProduct");

    // const OdDAIObjectIds& productIds = productIdSet->getArray();

    // qInfo() << "No of IFC products: " << productIds.length();

    // long totalPrinouts = 0;

    // for (OdDAIObjectIds::size_type iProduct = 0; iProduct < productIds.size(); ++iProduct)
    // {
    //     // if (totalPrinouts > 1)
    //     // {
    //     //     break;
    //     // }

    //     // if (iProduct > 1)
    //     // {
    //     //     break;
    //     // }

    //     OdIfc::OdIfcInstancePtr pEntity = productIds[iProduct].openObject();
    //     if (pEntity.isNull())
    //     {
    //         continue;
    //     }

    //     OdIfc::OdIfcProductPtr pProduct = OdIfc::OdIfcInstance::asCompound(pEntity);
    //     if (pProduct.isNull())
    //     {
    //         continue;
    //     }

    //     const OdIfc::OdIfcGeometricRepresentationItemPtrArray geomItems = pProduct->getGeometricRepresentationItems();
    //     // OdGeMatrix3d placementMatrix = pProduct->getObjectPlacement();

    //     if (geomItems.length() > 0)
    //     {
    //         qInfo() << "pEntity Type: " << pEntity->isA()->name();
    //         qInfo() << "pProduct Type: " << pProduct->isA()->name();
    //         // OdRxValue result = pEntity->getAttrCaseInsensitive("Name");
    //         // OdString nameValue;
    //         // result >> nameValue;
    //         // qInfo() << "Name: " << nameValue;

    //         // OdString entityType = pEntity->isA()->name();
    //         // qInfo() << "Class: " << entityType;

    //         // // OdRxValue globalIdResult = pEntity->getAttrCaseInsensitive("GlobalId");
    //         // // OdString globalIdValue;
    //         // // globalIdResult >> globalIdValue;
    //         // // qInfo() << "Name: " << nameValue;

    //         qInfo() << "Length of Gom items: " << geomItems.length();
    //         // // qInfo() << "LogicalLength of Gom items: " << geomItems.logicalLength();
    //         // // qInfo() << "Size of Gom items: " << geomItems.size();
    //         // // qInfo() << "Physial Length of Gom items: " << geomItems.physicalLength();

    //         // OdGeExtents3d ext;
    //         // OdResult response = pProduct->getGeomExtents(ext);
    //         // qInfo() << "Geom Extends: " << (response == eOk);
    //     }

    //     OdIfc::OdIfcRepresentationItemPtrArray repItems = pProduct->getRepresentationItems();
    //     // qInfo() << "Length of Rep Items: " << repItems.length();

    //     for (auto repItem: repItems)
    //     {
    //         if (repItem->type() == OdIfc::kIfcMappedItem)
    //         {

    //             OdIfc::OdIfcMappedItemPtr mappedItemPtr = OdIfc::OdIfcMappedItem::cast(repItem);

    //             qInfo() << "Mapped is null: " << mappedItemPtr.isNull();

    //             OdDAIObjectId mappingSource = mappedItemPtr->mappingSource();

    //             qInfo() << "Mapping Source is null: " << mappingSource.isNull();

    //             OdIfc::OdIfcInstancePtr pMappedEntity = mappingSource.openObject();

    //             qInfo() << "IFC Instance is null: " << pMappedEntity.isNull();

    //             OdIfc2x3::IfcRepresentationMapPtr mapPtr = OdIfc2x3::IfcRepresentationMap::cast(pMappedEntity);
    //             qInfo() << "Map Instance is null: " << mapPtr.isNull(); //pMappedEntity->isA()->name();

    //             OdDAIObjectId mappedRepresentation = mapPtr->getMappedRepresentation();
    //             qInfo() << "Mapped Representation is null: " << mappedRepresentation.isNull();

    //             OdIfc::OdIfcInstancePtr pMappedRep = mappedRepresentation.openObject();
    //             qInfo() << "Mapped Representation Pointer is null: " << pMappedRep.isNull();

    //             OdIfc2x3::IfcShapeRepresentationPtr shapePtr = OdIfc2x3::IfcShapeRepresentation::cast(pMappedRep);
    //             qInfo() << "Shape is null: " << shapePtr.isNull();

    //             OdDAIObjectIds shapeItems;
    //             shapePtr->getItems(shapeItems);

    //             qInfo() << shapeItems.size();

    //             OdIfc::OdIfcInstancePtr pShapeEntity = shapeItems[0].openObject();
    //             qInfo() << "pShapeEntity is null: " << pShapeEntity->isA()->name();

    //             try
    //             {
    //                 //OdIfc::OdIfcProductPtr pShapeProduct = OdIfc::OdIfcInstance::asCompound(pShapeEntity);

    //                 OdIfc::OdIfcGeometricRepresentationItemPtr pShapeProduct = OdIfc::OdIfcGeometricRepresentationItem::cast(OdIfc::OdIfcInstance::asCompound(pShapeEntity));

    //                 qInfo() << "pShapeProduct is null: " << pShapeProduct.isNull();
    //             }
    //             catch (const OdError& e)
    //             {
    //                 // std::wcout << e.description().c_str() << std::endl;
    //                 odPrintConsoleString(e.description());
    //             }

    //             // qInfo() << "pShapeProduct is null: " << pShapeProduct.isNull();

    //             // if (pMappedEntity.isNull())
    //             // {
    //             //     continue;
    //             // }

    //             // OdIfc::OdIfcProductPtr pMappedProduct = OdIfc::OdIfcInstance::asCompound(pMappedRep);

    //             // qInfo() << "IFC Product is null: " << pMappedProduct.isNull();
    //             // if (pMappedProduct.isNull())
    //             // {
    //             //     continue;
    //             // }

    //             // const OdIfc::OdIfcGeometricRepresentationItemPtrArray geomMapItems = pMappedProduct->getGeometricRepresentationItems();
    //             // qInfo() << "Length of Mapped Gom items: " << geomMapItems.length();
    //         }
    //     }

    // //     for (OdDAIObjectIds::size_type iItem = 0; iItem < geomItems.size(); ++iItem)
    // //     {
    // //         OdIfc::OdIfcGeometricRepresentationItemPtr pGeomItem = geomItems[iItem];
    // //         OdDAI::OdBodyVariant bodyContainer = pGeomItem->bodyContainer();

    // //         switch (bodyContainer.kind())
    // //         {
    // //         case OdDAI::OdBodyVariant::kFacetModelerBody:
    // //         {
    // //             //qInfo() << "Facet Modeler Body";
    // //             totalPrinouts = totalPrinouts + 1;

    // //             // OdDAI::ArrayOfOdDAIObjectId rels;

    // //             // pEntity->getInverseAttr("IsDefinedBy") >> rels;
    // //             // qInfo() << rels.getArray().length();

    // //             // OdString typeName = pEntity->getInstanceType()->name();
    // //             // odPrintConsoleString(typeName.c_str()); // in small letters only


    // //             // getting the verticies
    // //             const FacetModeler::Body* fBody = bodyContainer.facetModelerBody();
    // //             // qInfo() << "No of Faces: " << fBody->faceCount();
    // //             // qInfo() << "No of Vertices: " << fBody->vertexCount();

    // //             std::unordered_set<const FacetModeler::Vertex*> visitedVertex;
    // //             for (const FacetModeler::Vertex* vertex = fBody->vertexList(); vertex; vertex = vertex->next())
    // //             {
    // //                 if (visitedVertex.count(vertex)) {
    // //                     break;
    // //                 }
    // //                 visitedVertex.insert(vertex);

    // //                 //qInfo() << "        Vertex: " << vertex->point().x << "," << vertex->point().y << "," << vertex->point().z;
    // //             }


    // //             OdTvPointArray vertices = {};
    // //             OdInt32Array faces = {};

    // //             std::unordered_set<const FacetModeler::Face*> visited;
    // //             for (const FacetModeler::Face* face = fBody->faceList(); face; face = face->next())
    // //             {
    // //                 if (visited.count(face)) {
    // //                     break;
    // //                 }
    // //                 visited.insert(face);

    // //                 //qInfo() << "Face";

    // //                 //qInfo() << "    Loops Count: " << face->loopCount();

    // //                 uint numVerticies = 0;
    // //                 OdInt32Array faceLocal;
    // //                 std::unordered_set<const FacetModeler::Edge*> visitedEdge;
    // //                 for (const FacetModeler::Edge* edge = face->edge(0); edge; edge = edge->next())
    // //                 {
    // //                     if (visitedEdge.count(edge)) {
    // //                         break;
    // //                     }
    // //                     visitedEdge.insert(edge);

    // //                     double endX =  edge->endPoint().x;
    // //                     double endY =  edge->endPoint().y;
    // //                     double endZ =  edge->endPoint().z;
    // //                     double startX =  edge->startPoint().x;
    // //                     double startY =  edge->startPoint().y;
    // //                     double startZ =  edge->startPoint().z;

    // //                     if (entityType != "OdIfc4::IfcRoof")
    // //                     {
    // //                         // qInfo() << "        Edges: " << startX << "," << startY << "," << startZ << " End Edges: " << endX << "," << endY << "," << endZ;
    // //                     }

    // //                     numVerticies = numVerticies + 1;

    // //                     unsigned int vertIndex;
    // //                     if (vertices.find(edge->startPoint(), vertIndex))
    // //                     {
    // //                         faceLocal.append(vertIndex);
    // //                     }
    // //                     else
    // //                     {
    // //                         faceLocal.append(vertices.length());

    // //                         OdGePoint3d startPoint = edge->startPoint();
    // //                         OdGePoint3d updatedStartPoint = startPoint.transformBy(placementMatrix);

    // //                         vertices.append(updatedStartPoint);
    // //                     }
    // //                 }

    // //                 faces.append(numVerticies);

    // //                 for (int i = 0; i < faceLocal.length(); i++)
    // //                 {
    // //                     faces.append(faceLocal[i]);
    // //                 }
    // //             }

    // //             // if (entityType != "OdIfc4::IfcRoof")
    // //             // {
    // //             //     qInfo() << "Faces List";
    // //             //     for (int i = 0; i < faces.length(); i++)
    // //             //     {
    // //             //         qInfo() << faces.getAt(i);
    // //             //     }
    // //             // }

    // //             OdTvEntityId entityId0 = modelPtr->appendEntity(nameValue, &rc);
    // //             OdTvEntityPtr pTvEntity = entityId0.openObject(OdTv::kForWrite, &rc);



    // //             // get random number
    // //             // Create a random device to obtain a seed
    // //             std::random_device rd;

    // //             // Seed a Mersenne Twister engine
    // //             std::mt19937 gen(rd());

    // //             // Define a uniform integer distribution for numbers between 1 and 100
    // //             std::uniform_int_distribution<> distrib(1, 100);
    // //             int randomNumber = distrib(gen);
    // //             QString clashString = "-";
    // //             if (randomNumber > 85)
    // //             {
    // //                 double inrVal = randomNumber/40;
    // //                 QString numString = QString::number(inrVal);
    // //                 clashString = "IFC2x3::IFCBeam of value INR " + numString + "Lacs";
    // //             }

    // //             IFCDetail* ifcDetails = new IFCDetail(&engine);
    // //             ifcDetails->setName(QString::fromUtf16(nameValue.c_str()));
    // //             ifcDetails->setType(QString::fromUtf16(entityType.c_str()));
    // //             // ifcDetails->setGlobalId(QString::fromUtf16(globalIdValue.c_str()));
    // //             ifcDetails->setGlobalId(clashString);
    // //             ifcDetailList.append(ifcDetails);


    // //             OdTvGeometryDataId shellId = pTvEntity->appendShell(vertices, faces);
    // //             OdTvShellDataPtr shellPtr = shellId.openAsShell();

    // //             if (entityType == "OdIfc2x3::IfcWallStandardCase")
    // //             {
    // //                 OdTvColorDef entityColor;
    // //                 entityColor.setColor(188, 143, 143); //red
    // //                 pTvEntity->setColor(entityColor);
    // //             }
    // //             else if (entityType == "OdIfc2x3::IfcOpeningElement")
    // //             {
    // //                 OdTvColorDef entityColor;
    // //                 entityColor.setColor(165, 42, 42); //blue
    // //                 pTvEntity->setColor(entityColor);
    // //             }
    // //             else if (entityType == "OdIfc2x3::IfcBuildingElementProxy")
    // //             {
    // //                 OdTvColorDef entityColor;
    // //                 entityColor.setColor(70, 130, 180); //green
    // //                 pTvEntity->setColor(entityColor);
    // //             }
    // //             else if (entityType == "OdIfc2x3::IfcSlab")
    // //             {
    // //                 OdTvColorDef entityColor;
    // //                 entityColor.setColor(85, 107, 47); //yellow
    // //                 pTvEntity->setColor(entityColor);
    // //             }


    // //             break;
    // //         }
    // //         case OdDAI::OdBodyVariant::kMdBody:
    // //         {
    // //             //qInfo() << "Md Body";
    // //             totalPrinouts = totalPrinouts + 1;
    // //             break;
    // //         }
    // //         case OdDAI::OdBodyVariant::kAcisBody:
    // //         {
    // //             //qInfo() << "Acis Body";
    // //             totalPrinouts = totalPrinouts + 1;
    // //             break;
    // //         }
    // //         case OdDAI::OdBodyVariant::kBrep:
    // //         {
    // //             //qInfo() << "IFC Brep Body";
    // //             totalPrinouts = totalPrinouts + 1;
    // //             break;
    // //         }
    // //         case OdDAI::OdBodyVariant::kEmpty:
    // //         {
    // //             //qInfo() << "No Body";
    // //             totalPrinouts = totalPrinouts + 1;
    // //             break;
    // //         }
    // //         }
    // //     }
    // }

    // qInfo() << "Compose Entity Count: " << modelContext.getGeometryComposeTypes().length();
    // qInfo() << "IFCDetail Length: " << ifcDetailList.length();


    /*

        IFC CODE EXECUTION DONE
    */
    



    /*
    auto dbManager = DatabaseManager::getInstance();
    if (!dbManager->initializeDatabase("TestProject")) {
        qDebug() << "Failed to initialize database";
        return -1;
    }
    
    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();
    */

    
    BudgetHeadRepository* budgetHeadRepository = new BudgetHeadRepository(&engine);
    UnitOfMeasurementRepository* unitOfMeasurementRepository = new UnitOfMeasurementRepository(&engine);
    VendorRepository* vendorRepository = new VendorRepository(&engine);
    UserRepository* userRepository = new UserRepository(&engine);    
    MaterialRepository* materialRepository = new MaterialRepository(&engine);
    ScheduleSetupRepository* scheduleSetupRepository = new ScheduleSetupRepository(&engine);
    ScheduleOfRatesRepository* scheduleOfRatesRepository = new ScheduleOfRatesRepository(&engine);
    DraftEntityRepository* draftEntityRepository = new DraftEntityRepository(&engine);
    ProjectRepository* projectRepository = new ProjectRepository(&engine);
    IFCDetailRepository* ifcDetailRepository = new IFCDetailRepository(&engine);
    BIMElementRepository* bimElementRepository = new BIMElementRepository(&engine);
    BillOfQuantityRepository* billOfQuantityRepository = new BillOfQuantityRepository(&engine);
    TaskRepository* taskRepository = new TaskRepository(&engine);
    WorkOrderRepository* workOrderRepository = new WorkOrderRepository(&engine);
    WorkOrderLineRepository* workOrderLineRepository = new WorkOrderLineRepository(&engine);
    WorkBillingRepository* workBillingRepository = new WorkBillingRepository(&engine);
    WorkBillingLineRepository* workBillingLineRepository = new WorkBillingLineRepository(&engine);
    ProjectBudgetRepository* projectBudgetRepository = new ProjectBudgetRepository(&engine);
    FileRepository* fileRepository = new FileRepository(&engine);
    PurchaseOrderRepository* purchaseOrderRepository = new PurchaseOrderRepository(&engine);
    PurchaseOrderLineRepository* purchaseOrderLineRepository = new PurchaseOrderLineRepository(&engine);
    GoodReceivedNoteRepository* goodReceivedNoteRepository = new GoodReceivedNoteRepository(&engine);
    MaterialIndentRepository* materialIndentRepository = new MaterialIndentRepository(&engine);
    NetworkManager* networkManager = NetworkManager::getInstance();
   // IFCDetailRepository* ifcDetailRepository = new IFCDetailRepository(ifcDetailList, &engine);

    // UserRepository userRepo;
    // UnitOfMeasurementRepository uomRepo;
    
    // User testUser1(0, "user-123", true, "user123", "John Doe");
    // if (userRepo.save(testUser1)) {
    //     qDebug() << "User 1 saved successfully!";
    // }
    
    // User testUser2(0, "user-456", true, "user456", "Jane Smith");
    // if (userRepo.save(testUser2)) {
    //     qDebug() << "User 2 saved successfully!";
    // }
    
    // UnitOfMeasurement testUom1(0, "uom-001", true, "sqm", "area", 1.0, 0.0, 0.0, 0.0);
    // if (uomRepo.save(testUom1)) {
    //     qDebug() << "Unit of Measurement 1 (sqm) saved successfully!";
    // }
    
    // UnitOfMeasurement testUom2(0, "uom-002", true, "cubic meter", "volume", 0.0, 1.0, 0.0, 0.0);
    // if (uomRepo.save(testUom2)) {
    //     qDebug() << "Unit of Measurement 2 (cubic meter) saved successfully!";
    // }
    
    // qDebug() << "\n=== FETCHING ALL USERS ===";
    // auto users = userRepo.findAll();
    // qDebug() << "Total users found:" << users.size();
    
    // for (const auto& user : users) {
    //     qDebug() << "User ID:" << user->getId()
    //              << "Global ID:" << user->getGlobalId()
    //              << "Username:" << user->getUsername()
    //              << "User ID:" << user->getUserId()
    //              << "Approved:" << user->getApprovalStatus();
    // }
    
    // qDebug() << "\n=== FETCHING ALL UNITS OF MEASUREMENT ===";
    // auto uoms = uomRepo.findAll();
    // qDebug() << "Total UOMs found:" << uoms.size();
    
    // for (const auto& uom : uoms) {
    //     qDebug() << "UOM ID:" << uom->getId()
    //              << "Global ID:" << uom->getGlobalId()
    //              << "Name:" << uom->getUomName()
    //              << "Type:" << uom->getUnitType()
    //              << "Approved:" << uom->getApprovalStatus();
    // }
    
    // qDebug() << "\n=== TESTING INDIVIDUAL USER LOOKUP ===";
    // auto foundUser = userRepo.findByUsername("John Doe");
    // if (foundUser) {
    //     qDebug() << "Found user by username:" << foundUser->getUsername()
    //              << "with User ID:" << foundUser->getUserId();
    // } else {
    //     qDebug() << "User 'John Doe' not found!";
    // }
    
    // qDebug() << "\n=== TESTING APPROVED USERS ONLY ===";
    // auto approvedUsers = userRepo.findByApprovalStatus(true);
    // qDebug() << "Approved users count:" << approvedUsers.size();
    
    // qDebug() << "Construction Management Tool - Model Layer Test Complete";
    
    //engine.rootContext()->setContextProperty("treeModel", treeModel);
    //engine.rootContext()->setContextProperty("ifcDetailRepository", ifcDetailRepository);

    engine.rootContext()->setContextProperty("budgetHeadRepository", budgetHeadRepository);
    engine.rootContext()->setContextProperty("unitOfMeasurementRepository", unitOfMeasurementRepository);
    engine.rootContext()->setContextProperty("vendorRepository", vendorRepository);
    engine.rootContext()->setContextProperty("userRepository", userRepository);   
    engine.rootContext()->setContextProperty("materialRepository", materialRepository);
    engine.rootContext()->setContextProperty("scheduleSetupRepository", scheduleSetupRepository);
    engine.rootContext()->setContextProperty("scheduleOfRatesRepository", scheduleOfRatesRepository);
    engine.rootContext()->setContextProperty("draftEntityRepository", draftEntityRepository);
    engine.rootContext()->setContextProperty("projectRepository", projectRepository);
    engine.rootContext()->setContextProperty("ifcDetailRepository", ifcDetailRepository);
    engine.rootContext()->setContextProperty("taskRepository", taskRepository);
    engine.rootContext()->setContextProperty("workOrderRepository", workOrderRepository);
    engine.rootContext()->setContextProperty("workOrderLineRepository", workOrderLineRepository);
    engine.rootContext()->setContextProperty("workBillingRepository", workBillingRepository);
    engine.rootContext()->setContextProperty("workBillingLineRepository", workBillingLineRepository);
    engine.rootContext()->setContextProperty("projectBudgetRepository", projectBudgetRepository);
    engine.rootContext()->setContextProperty("fileRepository", fileRepository);
    engine.rootContext()->setContextProperty("purchaseOrderRepository", purchaseOrderRepository);
    engine.rootContext()->setContextProperty("purchaseOrderLineRepository", purchaseOrderLineRepository);
    engine.rootContext()->setContextProperty("goodReceivedNoteRepository", goodReceivedNoteRepository);
    engine.rootContext()->setContextProperty("materialIndentRepository", materialIndentRepository);

    engine.rootContext()->setContextProperty("bimElementRepository", bimElementRepository);
    engine.rootContext()->setContextProperty("billofQuantityRepository", billOfQuantityRepository);
    engine.rootContext()->setContextProperty("networkManager", networkManager);





    qmlRegisterType<MyGLItem>("com.fh.models", 1, 0, "GLScene");
    qmlRegisterType<BudgetHead>("com.fh.models", 1, 0, "BudgetHead");
    qmlRegisterType<UnitOfMeasurement>("com.fh.models", 1, 0, "UnitOfMeasurement");
    qmlRegisterType<Material>("com.fh.models", 1, 0, "Material");
    qmlRegisterType<Task>("com.fh.models", 1, 0, "Task");
    qmlRegisterType<Vendor>("com.fh.models", 1, 0, "Vendor");
    qmlRegisterType<User>("com.fh.models", 1, 0, "User");   
    qmlRegisterType<ScheduleSetup>("com.fh.models", 1, 0, "ScheduleSetup");
    qmlRegisterType<ScheduleOfRates>("com.fh.models", 1, 0, "ScheduleOfRates");
    qmlRegisterType<WorkOrder>("com.fh.models", 1, 0, "WorkOrder");
    qmlRegisterType<WorkOrderLine>("com.fh.models", 1, 0, "WorkOrderLine");
    qmlRegisterType<WorkBilling>("com.fh.models", 1, 0, "WorkBilling");
    qmlRegisterType<WorkBillingLine>("com.fh.models", 1, 0, "WorkBillingLine");
    qmlRegisterType<ProjectBudget>("com.fh.models", 1, 0, "ProjectBudget");
    qmlRegisterType<File>("com.fh.models", 1, 0, "File");
    qmlRegisterType<PurchaseOrder>("com.fh.models", 1, 0, "PurchaseOrder");
    qmlRegisterType<PurchaseOrderLine>("com.fh.models", 1, 0, "PurchaseOrderLine");
    qmlRegisterType<GoodReceivedNote>("com.fh.models", 1, 0, "GoodReceivedNote");
    qmlRegisterType<MaterialIndent>("com.fh.models", 1, 0, "MaterialIndent");


    qmlRegisterType<DraftEntity>("com.fh.models", 1, 0, "DraftEntity");
    qmlRegisterType<User>("com.fh.models", 1, 0, "Project");

    qRegisterMetaType<HelperPoint>();


    qmlRegisterType<UserController>("com.fh.controllers", 1, 0, "UserController");
    qmlRegisterType<ProjectController>("com.fh.controllers", 1, 0, "ProjectController");
    qmlRegisterType<BudgetHeadController>("com.fh.controllers", 1, 0, "BudgetHeadController");
    qmlRegisterType<TaskController>("com.fh.controllers", 1, 0, "TaskController");
    qmlRegisterType<VendorController>("com.fh.controllers", 1, 0, "VendorController");
    qmlRegisterType<MaterialController>("com.fh.controllers", 1, 0, "MaterialController");
    qmlRegisterType<UnitOfMeasurementController>("com.fh.controllers", 1, 0, "UnitOfMeasurementController");
    qmlRegisterType<ScheduleSetupController>("com.fh.controllers", 1, 0, "ScheduleSetupController");
    qmlRegisterType<ScheduleOfRatesController>("com.fh.controllers", 1, 0, "ScheduleOfRatesController");
    qmlRegisterType<IFCDetailController>("com.fh.controllers", 1, 0, "IFCDetailController");
    qmlRegisterType<IFCWallController>("com.fh.controllers", 1, 0, "IFCWallController");
    qmlRegisterType<BIMElementController>("com.fh.controllers", 1, 0, "BIMElementController");
    qmlRegisterType<BillOfQuantityController>("com.fh.controllers", 1, 0, "BillOfQuantityController");
    qmlRegisterType<BillOfQuantityLineController>("com.fh.controllers", 1, 0, "BillOfQuantityLineController");
    qmlRegisterType<WorkOrderController>("com.fh.controllers", 1, 0, "WorkOrderController");
    qmlRegisterType<WorkOrderLineController>("com.fh.controllers", 1, 0, "WorkOrderLineController");

    qmlRegisterType<WorkBillingController>("com.fh.controllers", 1, 0, "WorkBillingController");
    qmlRegisterType<WorkBillingLineController>("com.fh.controllers", 1, 0, "WorkBillingLineController");
    qmlRegisterType<ProjectBudgetController>("com.fh.controllers", 1, 0, "ProjectBudgetController");
    qmlRegisterType<FileController>("com.fh.controllers", 1, 0, "FileController");
    qmlRegisterType<PurchaseOrderController>("com.fh.controllers", 1, 0, "PurchaseOrderController");
    qmlRegisterType<PurchaseOrderLineController>("com.fh.controllers", 1, 0, "PurchaseOrderLineController");
    qmlRegisterType<GoodReceivedNoteController>("com.fh.controllers", 1, 0, "GoodReceivedNoteController");
    qmlRegisterType<MaterialIndentController>("com.fh.controllers", 1, 0, "MaterialIndentController");
    qmlRegisterType<DraftEntityController>("com.fh.controllers", 1, 0, "DraftEntityController");

    const QUrl url(QStringLiteral("qrc:/resources/QML/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);
    
    int result = app.exec();

    odTvUninitialize();
    odIfcUninitialize();
    odrxUninitialize();

    // api_stop_modeller();

    return result;
}
