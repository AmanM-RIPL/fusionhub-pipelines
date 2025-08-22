cmake_minimum_required(VERSION 3.16)

project(FusionHubQt VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL")
# set(CMAKE_GENERATOR_PLATFORM x64)  not required if using Ninja but required if using VS 2022 as CMAKE Generator

add_compile_options(-w)
add_compile_options(-I "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Sdai\\Include")


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql Qml Quick Charts)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    src/main.cpp
    src/database/database_manager.cpp
    src/models/user.cpp
    src/models/unit_of_measurement.cpp
    src/models/vendor.cpp
    src/models/material.cpp
    src/models/budget_head.cpp
    src/models/schedule_setup.cpp
    src/models/task.cpp
    src/models/task_image.cpp
    src/models/task_measurement.cpp
    src/models/file.cpp
    src/models/file_permission.cpp
    src/models/schedule_of_rates.cpp
    src/models/schedule_of_rates_line.cpp
    src/models/bill_of_quantity.cpp
    src/models/bill_of_quantity_line.cpp
    src/models/project_budget.cpp
    src/models/work_order.cpp
    src/models/work_order_line.cpp
    src/models/work_billing.cpp
    src/models/work_billing_line.cpp
    src/models/purchase_order.cpp
    src/models/purchase_order_line.cpp
    src/models/good_received_note.cpp
    src/models/material_indent.cpp
    src/repositories/abstract_repository.cpp
    src/repositories/user_repository.cpp
    src/repositories/unit_of_measurement_repository.cpp
    src/repositories/vendor_repository.cpp
    src/repositories/material_repository.cpp
    src/repositories/budget_head_repository.cpp
    src/repositories/schedule_setup_repository.cpp
    src/repositories/task_repository.cpp
    src/repositories/task_image_repository.cpp
    src/repositories/task_measurement_repository.cpp
    src/repositories/file_repository.cpp
    src/repositories/file_permission_repository.cpp
    src/repositories/schedule_of_rates_repository.cpp
    src/repositories/schedule_of_rates_line_repository.cpp
    src/repositories/bill_of_quantity_repository.cpp
    src/repositories/bill_of_quantity_line_repository.cpp
    src/repositories/project_budget_repository.cpp
    src/repositories/work_order_repository.cpp
    src/repositories/work_order_line_repository.cpp
    src/repositories/work_billing_repository.cpp
    src/repositories/work_billing_line_repository.cpp
    src/repositories/purchase_order_repository.cpp
    src/repositories/purchase_order_line_repository.cpp
    src/repositories/good_received_note_repository.cpp
    src/repositories/material_indent_repository.cpp
    src/repositories/ifcdetailrepository.cpp
    src/common/ifcdetail.cpp
    src/common/myglitem.cpp
)

set(HEADERS
    src/database/database_manager.h
    src/models/user.h
    src/models/unit_of_measurement.h
    src/models/vendor.h
    src/models/material.h
    src/models/budget_head.h
    src/models/schedule_setup.h
    src/models/task.h
    src/models/task_image.h
    src/models/task_measurement.h
    src/models/file.h
    src/models/file_permission.h
    src/models/schedule_of_rates.h
    src/models/schedule_of_rates_line.h
    src/models/bill_of_quantity.h
    src/models/bill_of_quantity_line.h
    src/models/project_budget.h
    src/models/work_order.h
    src/models/work_order_line.h
    src/models/work_billing.h
    src/models/work_billing_line.h
    src/models/purchase_order.h
    src/models/purchase_order_line.h
    src/models/good_received_note.h
    src/models/material_indent.h
    src/repositories/abstract_repository.h
    src/repositories/user_repository.h
    src/repositories/unit_of_measurement_repository.h
    src/repositories/vendor_repository.h
    src/repositories/material_repository.h
    src/repositories/budget_head_repository.h
    src/repositories/schedule_setup_repository.h
    src/repositories/task_repository.h
    src/repositories/task_image_repository.h
    src/repositories/task_measurement_repository.h
    src/repositories/file_repository.h
    src/repositories/file_permission_repository.h
    src/repositories/schedule_of_rates_repository.h
    src/repositories/schedule_of_rates_line_repository.h
    src/repositories/bill_of_quantity_repository.h
    src/repositories/bill_of_quantity_line_repository.h
    src/repositories/project_budget_repository.h
    src/repositories/work_order_repository.h
    src/repositories/work_order_line_repository.h
    src/repositories/work_billing_repository.h
    src/repositories/work_billing_line_repository.h
    src/repositories/purchase_order_repository.h
    src/repositories/purchase_order_line_repository.h
    src/repositories/good_received_note_repository.h
    src/repositories/material_indent_repository.h
    src/repositories/ifcdetailrepository.h
    src/common/ifcdetail.h
    src/common/myglitem.h
)

add_executable(FusionHubQt ${SOURCES} ${HEADERS}
    src/Resources.qrc
    src/common/repository_locator.h src/common/repository_locator.cpp
    src/controllers/user_controller.h src/controllers/user_controller.cpp

)

target_include_directories(FusionHubQt PRIVATE src)

target_link_libraries(FusionHubQt
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Sql
        Qt6::Qml
        Qt6::Quick
        Qt6::Charts
        Qt6::OpenGL
)

if(WIN32)
    set_target_properties(FusionHubQt PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

if(APPLE)
    set_target_properties(FusionHubQt PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()


target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\Kernel\\Include")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\FacetModeler\\include")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\FacetModeler\\include\\Modeler")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\Visualize\\Include")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Sdai\\Include")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Ifc\\Include")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Ifc\\Include\\Common")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Ifc\\Examples\\Common")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Ifc\\Extensions\\ExServices")

target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\KernelBase\\Include")

target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\ThirdParty\\activation")
target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\ThirdParty\\activation")

target_include_directories(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\Kernel\\Extensions\\ExServices")

target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_Alloc.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_Root.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_DbRoot.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_Br.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_Gi.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\FacetModeler.lib")

target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TD_Ge.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TV_CoreDb.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TV_DbCore.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TV_Visualize.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Visualize_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\TV_VisualizeTools.lib")

target_sources(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\Kernel\\Extensions\\ExServices\\ExSystemServices.cpp")
target_sources(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\Kernel\\Extensions\\ExServices\\RxSystemServicesImpl.cpp")
target_sources(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\Kernel_vc17_amd64dll_26.6\\Kernel\\Extensions\\ExServices\\OdFileBuf.cpp")
target_sources(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\Ifc\\Extensions\\ExServices\\ExIfcHostAppServices.cpp")

target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\IfcCore.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\IfcCoreTools.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\IfcGeom.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\hdf5.lib")
target_link_libraries(FusionHubQt PRIVATE "C:\\Users\\RIPL\\Documents\\ODA\\IFC_vc17_amd64dll_26.6\\lib\\vc17_amd64dll\\sdai.lib")
target_link_libraries(FusionHubQt PRIVATE Qt6::Core)
target_link_libraries(FusionHubQt PRIVATE Qt6::Core)
target_link_libraries(FusionHubQt PRIVATE Qt6::Core)
