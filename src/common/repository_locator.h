#ifndef REPOSITORY_LOCATOR_H
#define REPOSITORY_LOCATOR_H

#include "repositories/user_repository.h"
#include "repositories/project_repository.h"
#include "repositories/bim_element_repository.h"
#include "repositories/bim_parameter_repository.h"
#include "repositories/budget_head_repository.h"
#include "repositories/project_budget_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/material_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/schedule_setup_repository.h"
#include "repositories/schedule_of_rates_repository.h"
#include "repositories/schedule_of_rates_line_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/ifc_detail_repository.h"
#include "repositories/ifc_repositories/ifc_wall_repository.h"
#include "repositories/bill_of_quantity_repository.h"
#include "repositories/bill_of_quantity_line_repository.h"
#include "repositories/task_repository.h"
#include "repositories/work_order_repository.h"
#include "repositories/work_order_line_repository.h"
#include "repositories/file_repository.h"
#include "repositories/purchase_order_repository.h"
#include "repositories/purchase_order_line_repository.h"
#include "repositories/good_received_note_repository.h"
#include "repositories/material_indent_repository.h"
#include "repositories/work_billing_repository.h"
#include "repositories/work_billing_line_repository.h"

class RepositoryLocator
{
public:
    RepositoryLocator();

    static RepositoryLocator& instance();

    UserRepository* userRepository();
    ProjectRepository* projectRepository();
    BIMElementRepository* bimElementRepository();
    BIMParameterRepository* bimParameterRepository();
    BudgetHeadRepository* budgetHeadRepository();
    VendorRepository* vendorRepository();
    MaterialRepository* materialRepository();
    UnitOfMeasurementRepository* unitOfMeasurementRepository();
    ScheduleSetupRepository* scheduleSetupRepository();
    ScheduleOfRatesRepository* scheduleOfRatesRepository();
    ScheduleOfRatesLineRepository* scheduleOfRatesLineRepository();
    DraftEntityRepository* draftEntityRepository();
    IFCDetailRepository* ifcDetailRepository();
    BillOfQuantityRepository* billOfQuantityRepository();
    BillOfQuantityLineRepository* billOfQuantityLineRepository();
    TaskRepository* taskRepository();
    WorkOrderRepository* workOrderRepository();
    WorkOrderLineRepository* workOrderLineRepository();
    ProjectBudgetRepository* projectBudgetRepository();
    FileRepository* fileRepository();
    PurchaseOrderRepository* purchaseOrderRepository();
    PurchaseOrderLineRepository* purchaseOrderLineRepository();
    GoodReceivedNoteRepository* goodReceivedNoteRepository();
    MaterialIndentRepository* materialIndentRepository();
    WorkBillingRepository* workBillingRepository();
    WorkBillingLineRepository* workBillingLineRepository();

    IFCWallRepository* ifcWallRepository();




private:
    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<ProjectRepository> m_projectRepository;
    std::unique_ptr<BIMElementRepository> m_bimElementRepository;
    std::unique_ptr<BIMParameterRepository> m_bimParameterRepository;
    std::unique_ptr<BudgetHeadRepository> m_budgetHeadRepository;
    std::unique_ptr<VendorRepository> m_vendorRepository;
    std::unique_ptr<MaterialRepository> m_materialRepository;
    std::unique_ptr<UnitOfMeasurementRepository> m_unitOfMeasurementRepository;
    std::unique_ptr<ScheduleOfRatesRepository> m_scheduleOfRatesRepository;
    std::unique_ptr<ScheduleOfRatesLineRepository> m_scheduleOfRatesLineRepository;
    std::unique_ptr<ScheduleSetupRepository> m_scheduleSetupRepository;
    std::unique_ptr<DraftEntityRepository> m_draftEntityRepository;
    std::unique_ptr<IFCDetailRepository> m_ifcDetailRepository;
    std::unique_ptr<BillOfQuantityRepository> m_billOfQuantityRepository;
    std::unique_ptr<TaskRepository> m_taskRepository;
    std::unique_ptr<WorkOrderRepository> m_workOrderRepository;
    std::unique_ptr<WorkOrderLineRepository> m_workOrderLineRepository;
    std::unique_ptr<FileRepository> m_fileRepository;
    std::unique_ptr<PurchaseOrderRepository> m_purchaseOrderRepository;
    std::unique_ptr<PurchaseOrderLineRepository> m_purchaseOrderLineRepository;
    std::unique_ptr<GoodReceivedNoteRepository> m_goodReceivedNoteRepository;

    std::unique_ptr<BillOfQuantityLineRepository> m_billOfQuantityLineRepository;
    std::unique_ptr<ProjectBudgetRepository> m_projectBudgetRepository;
    std::unique_ptr<MaterialIndentRepository> m_materialIndentRepository;

    std::unique_ptr<WorkBillingRepository> m_workBillingRepository;
    std::unique_ptr<WorkBillingLineRepository> m_workBillingLineRepository;

    std::unique_ptr<IFCWallRepository> m_ifcWallRepository;

};

#endif // REPOSITORY_LOCATOR_H
