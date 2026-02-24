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
#include "controllers/schedule_of_rates_line_controller.h"

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
    qmlRegisterType<ScheduleOfRatesLine>("com.fh.models", 1, 0, "ScheduleOfRatesLine");
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
    qmlRegisterType<ScheduleOfRatesLineController>("com.fh.controllers", 1, 0, "ScheduleOfRatesLineController");
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
