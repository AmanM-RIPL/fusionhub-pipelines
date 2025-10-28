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

#include "common/myglitem.h"
#include "common/ifcdetail.h"

#include "database/database_manager.h"

#include "repositories/user_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/budget_head_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/material_repository.h"
#include "repositories/schedule_setup_repository.h"
#include "repositories/ifc_detail_repository.h"
#include "repositories/draft_entity_repository.h"

#include "repositories/ifc_detail_repository.h"

#include "controllers/user_controller.h"
#include "controllers/project_controller.h"
#include "controllers/budget_head_controller.h"
#include "controllers/vendor_controller.h"
#include "controllers/material_controller.h"
#include "controllers/unit_of_measurement_controller.h"
#include "controllers/schedule_setup_controller.h"
#include "controllers/ifc_detail_controller.h"

#include "models/user.h"
#include "models/unit_of_measurement.h"
#include "models/draft_entity.h"

// There has to be a better way???????????
const OdString OdString::kEmpty;
const OdDAIObjectId OdDAIObjectId::kNull;
OdArrayBuffer OdArrayBuffer::g_empty_array_buffer;
OdGeTol OdGeContext::gTol;

//Create global state later
std::shared_ptr<User> gUser = std::make_shared<User>();
int gTenantId = 0;
int gProjectId = 0;
QString gProjectName ="";
QString gEnvironmentPath = "C:\\Users\\RIPL\\Documents\\FusionHubData";
OdStaticRxObject<MyServices> svcs;

int main(int argc, char *argv[])
{
    //static OdStaticRxObject<MyServices> svcs;

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


    // // Read IFC File
    // OdString ifcFileName("C:\\Users\\RIPL\\Downloads\\BasicHouse.ifc");

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

    // // Compose IFC geometry
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
    //     // if (totalPrinouts > 10)
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

    //     OdRxValue result = pEntity->getAttrCaseInsensitive("Name");
    //     OdString nameValue;
    //     result >> nameValue;
    //     //qInfo() << "Name: " << nameValue;

    //     OdString entityType = pEntity->isA()->name();
    //     //qInfo() << "Class: " << entityType;

    //     OdRxValue globalIdResult = pEntity->getAttrCaseInsensitive("GlobalId");
    //     OdString globalIdValue;
    //     globalIdResult >> globalIdValue;
    //     //qInfo() << "Name: " << nameValue;

    //     const OdIfc::OdIfcGeometricRepresentationItemPtrArray geomItems = pProduct->getGeometricRepresentationItems();
    //     OdGeMatrix3d placementMatrix = pProduct->getObjectPlacement();

    //     //qInfo() << "Length of Gom items: " << geomItems.length();
    //     //qInfo() << "LogicalLength of Gom items: " << geomItems.logicalLength();
    //     //qInfo() << "Size of Gom items: " << geomItems.size();
    //     //qInfo() << "Physial Length of Gom items: " << geomItems.physicalLength();

    //     for (OdDAIObjectIds::size_type iItem = 0; iItem < geomItems.size(); ++iItem)
    //     {
    //         OdIfc::OdIfcGeometricRepresentationItemPtr pGeomItem = geomItems[iItem];
    //         OdDAI::OdBodyVariant bodyContainer = pGeomItem->bodyContainer();

    //         switch (bodyContainer.kind())
    //         {
    //         case OdDAI::OdBodyVariant::kFacetModelerBody:
    //         {
    //             //qInfo() << "Facet Modeler Body";
    //             totalPrinouts = totalPrinouts + 1;

    //             // OdDAI::ArrayOfOdDAIObjectId rels;

    //             // pEntity->getInverseAttr("IsDefinedBy") >> rels;
    //             // qInfo() << rels.getArray().length();

    //             // OdString typeName = pEntity->getInstanceType()->name();
    //             // odPrintConsoleString(typeName.c_str()); // in small letters only


    //             // getting the verticies
    //             const FacetModeler::Body* fBody = bodyContainer.facetModelerBody();
    //             // qInfo() << "No of Faces: " << fBody->faceCount();
    //             // qInfo() << "No of Vertices: " << fBody->vertexCount();

    //             std::unordered_set<const FacetModeler::Vertex*> visitedVertex;
    //             for (const FacetModeler::Vertex* vertex = fBody->vertexList(); vertex; vertex = vertex->next())
    //             {
    //                 if (visitedVertex.count(vertex)) {
    //                     break;
    //                 }
    //                 visitedVertex.insert(vertex);

    //                 //qInfo() << "        Vertex: " << vertex->point().x << "," << vertex->point().y << "," << vertex->point().z;
    //             }


    //             OdTvPointArray vertices = {};
    //             OdInt32Array faces = {};

    //             std::unordered_set<const FacetModeler::Face*> visited;
    //             for (const FacetModeler::Face* face = fBody->faceList(); face; face = face->next())
    //             {
    //                 if (visited.count(face)) {
    //                     break;
    //                 }
    //                 visited.insert(face);

    //                 //qInfo() << "Face";

    //                 //qInfo() << "    Loops Count: " << face->loopCount();

    //                 uint numVerticies = 0;
    //                 OdInt32Array faceLocal;
    //                 std::unordered_set<const FacetModeler::Edge*> visitedEdge;
    //                 for (const FacetModeler::Edge* edge = face->edge(0); edge; edge = edge->next())
    //                 {
    //                     if (visitedEdge.count(edge)) {
    //                         break;
    //                     }
    //                     visitedEdge.insert(edge);

    //                     double endX =  edge->endPoint().x;
    //                     double endY =  edge->endPoint().y;
    //                     double endZ =  edge->endPoint().z;
    //                     double startX =  edge->startPoint().x;
    //                     double startY =  edge->startPoint().y;
    //                     double startZ =  edge->startPoint().z;

    //                     if (entityType != "OdIfc4::IfcRoof")
    //                     {
    //                         // qInfo() << "        Edges: " << startX << "," << startY << "," << startZ << " End Edges: " << endX << "," << endY << "," << endZ;
    //                     }

    //                     numVerticies = numVerticies + 1;

    //                     unsigned int vertIndex;
    //                     if (vertices.find(edge->startPoint(), vertIndex))
    //                     {
    //                         faceLocal.append(vertIndex);
    //                     }
    //                     else
    //                     {
    //                         faceLocal.append(vertices.length());

    //                         OdGePoint3d startPoint = edge->startPoint();
    //                         OdGePoint3d updatedStartPoint = startPoint.transformBy(placementMatrix);

    //                         vertices.append(updatedStartPoint);
    //                     }
    //                 }

    //                 faces.append(numVerticies);

    //                 for (int i = 0; i < faceLocal.length(); i++)
    //                 {
    //                     faces.append(faceLocal[i]);
    //                 }
    //             }

    //             // if (entityType != "OdIfc4::IfcRoof")
    //             // {
    //             //     qInfo() << "Faces List";
    //             //     for (int i = 0; i < faces.length(); i++)
    //             //     {
    //             //         qInfo() << faces.getAt(i);
    //             //     }
    //             // }

    //             OdTvEntityId entityId0 = modelPtr->appendEntity(nameValue, &rc);
    //             OdTvEntityPtr pTvEntity = entityId0.openObject(OdTv::kForWrite, &rc);



    //             // get random number
    //             // Create a random device to obtain a seed
    //             std::random_device rd;

    //             // Seed a Mersenne Twister engine
    //             std::mt19937 gen(rd());

    //             // Define a uniform integer distribution for numbers between 1 and 100
    //             std::uniform_int_distribution<> distrib(1, 100);
    //             int randomNumber = distrib(gen);
    //             QString clashString = "-";
    //             if (randomNumber > 85)
    //             {
    //                 double inrVal = randomNumber/40;
    //                 QString numString = QString::number(inrVal);
    //                 clashString = "IFC2x3::IFCBeam of value INR " + numString + "Lacs";
    //             }

    //             IFCDetail* ifcDetails = new IFCDetail(&engine);
    //             ifcDetails->setName(QString::fromUtf16(nameValue.c_str()));
    //             ifcDetails->setType(QString::fromUtf16(entityType.c_str()));
    //             // ifcDetails->setGlobalId(QString::fromUtf16(globalIdValue.c_str()));
    //             ifcDetails->setGlobalId(clashString);
    //             ifcDetailList.append(ifcDetails);


    //             OdTvGeometryDataId shellId = pTvEntity->appendShell(vertices, faces);
    //             OdTvShellDataPtr shellPtr = shellId.openAsShell();

    //             if (entityType == "OdIfc2x3::IfcWallStandardCase")
    //             {
    //                 OdTvColorDef entityColor;
    //                 entityColor.setColor(188, 143, 143); //red
    //                 pTvEntity->setColor(entityColor);
    //             }
    //             else if (entityType == "OdIfc2x3::IfcOpeningElement")
    //             {
    //                 OdTvColorDef entityColor;
    //                 entityColor.setColor(165, 42, 42); //blue
    //                 pTvEntity->setColor(entityColor);
    //             }
    //             else if (entityType == "OdIfc2x3::IfcBuildingElementProxy")
    //             {
    //                 OdTvColorDef entityColor;
    //                 entityColor.setColor(70, 130, 180); //green
    //                 pTvEntity->setColor(entityColor);
    //             }
    //             else if (entityType == "OdIfc2x3::IfcSlab")
    //             {
    //                 OdTvColorDef entityColor;
    //                 entityColor.setColor(85, 107, 47); //yellow
    //                 pTvEntity->setColor(entityColor);
    //             }


    //             break;
    //         }
    //         case OdDAI::OdBodyVariant::kMdBody:
    //         {
    //             //qInfo() << "Md Body";
    //             totalPrinouts = totalPrinouts + 1;
    //             break;
    //         }
    //         case OdDAI::OdBodyVariant::kAcisBody:
    //         {
    //             //qInfo() << "Acis Body";
    //             totalPrinouts = totalPrinouts + 1;
    //             break;
    //         }
    //         case OdDAI::OdBodyVariant::kBrep:
    //         {
    //             //qInfo() << "IFC Brep Body";
    //             totalPrinouts = totalPrinouts + 1;
    //             break;
    //         }
    //         case OdDAI::OdBodyVariant::kEmpty:
    //         {
    //             //qInfo() << "No Body";
    //             totalPrinouts = totalPrinouts + 1;
    //             break;
    //         }
    //         }
    //     }
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
    DraftEntityRepository* draftEntityRepository = new DraftEntityRepository(&engine);
    ProjectRepository* projectRepository = new ProjectRepository(&engine);
    IFCDetailRepository* ifcDetailRepository = new IFCDetailRepository(&engine);

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
    engine.rootContext()->setContextProperty("draftEntityRepository", draftEntityRepository);
    engine.rootContext()->setContextProperty("projectRepository", projectRepository);
    engine.rootContext()->setContextProperty("ifcDetailRepository", ifcDetailRepository);


    qmlRegisterType<MyGLItem>("com.fh.models", 1, 0, "GLScene");
    qmlRegisterType<BudgetHead>("com.fh.models", 1, 0, "BudgetHead");
    qmlRegisterType<UnitOfMeasurement>("com.fh.models", 1, 0, "UnitOfMeasurement");
    qmlRegisterType<Material>("com.fh.models", 1, 0, "Material");
    qmlRegisterType<Vendor>("com.fh.models", 1, 0, "Vendor");
    qmlRegisterType<User>("com.fh.models", 1, 0, "User");   
    qmlRegisterType<ScheduleSetup>("com.fh.models", 1, 0, "ScheduleSetup");
    qmlRegisterType<DraftEntity>("com.fh.models", 1, 0, "DraftEntity");
    qmlRegisterType<User>("com.fh.models", 1, 0, "Project");


    qmlRegisterType<UserController>("com.fh.controllers", 1, 0, "UserController");
    qmlRegisterType<ProjectController>("com.fh.controllers", 1, 0, "ProjectController");
    qmlRegisterType<BudgetHeadController>("com.fh.controllers", 1, 0, "BudgetHeadController");
    qmlRegisterType<VendorController>("com.fh.controllers", 1, 0, "VendorController");
    qmlRegisterType<MaterialController>("com.fh.controllers", 1, 0, "MaterialController");
    qmlRegisterType<UnitOfMeasurementController>("com.fh.controllers", 1, 0, "UnitOfMeasurementController");
    qmlRegisterType<ScheduleSetupController>("com.fh.controllers", 1, 0, "ScheduleSetupController");
    qmlRegisterType<IFCDetailController>("com.fh.controllers", 1, 0, "IFCDetailController");

    
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

    return result;
}
