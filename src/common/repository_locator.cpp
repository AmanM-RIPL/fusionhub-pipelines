#include "repository_locator.h"

RepositoryLocator::RepositoryLocator() {
    m_userRepository = std::make_unique<UserRepository>();
    m_projectRepository = std::make_unique<ProjectRepository>();
    m_bimElementRepository = std::make_unique<BIMElementRepository>();
    m_bimParameterRepository = std::make_unique<BIMParameterRepository>();
    m_budgetHeadRepository = std::make_unique<BudgetHeadRepository>();
    m_unitOfMeasurementRepository = std::make_unique<UnitOfMeasurementRepository>();
    m_vendorRepository =  std::make_unique<VendorRepository>();
    m_materialRepository = std::make_unique<MaterialRepository>();
    m_scheduleSetupRepository = std::make_unique<ScheduleSetupRepository>();
    m_scheduleOfRatesRepository = std::make_unique<ScheduleOfRatesRepository>();
    m_draftEntityRepository = std::make_unique<DraftEntityRepository>();
    m_ifcDetailRepository = std::make_unique<IFCDetailRepository>();
    m_billOfQuantityRepository = std::make_unique<BillOfQuantityRepository>();
    m_billOfQuantityLineRepository = std::make_unique<BillOfQuantityLineRepository>();
    m_taskRepository = std::make_unique<TaskRepository>();
    m_workOrderRepository = std::make_unique<WorkOrderRepository>();
    m_projectBudgetRepository = std::make_unique<ProjectBudgetRepository>();
    m_fileRepository = std::make_unique<FileRepository>();
    m_purchaseOrderRepository = std::make_unique<PurchaseOrderRepository>();
}

RepositoryLocator &RepositoryLocator::instance()
{
    static RepositoryLocator locator;
    return locator;
}

UserRepository *RepositoryLocator::userRepository()
{
    return m_userRepository.get();
}

ProjectRepository *RepositoryLocator::projectRepository()
{
    return m_projectRepository.get();
}

BIMElementRepository *RepositoryLocator::bimElementRepository()
{
    return m_bimElementRepository.get();
}

BIMParameterRepository *RepositoryLocator::bimParameterRepository()
{
    return m_bimParameterRepository.get();
}

BudgetHeadRepository *RepositoryLocator::budgetHeadRepository()
{
    return m_budgetHeadRepository.get();
}

VendorRepository *RepositoryLocator::vendorRepository()
{
    return m_vendorRepository.get();
}

MaterialRepository *RepositoryLocator::materialRepository()
{
    return m_materialRepository.get();
}

UnitOfMeasurementRepository *RepositoryLocator::unitOfMeasurementRepository()
{
    return m_unitOfMeasurementRepository.get();
}

ScheduleSetupRepository *RepositoryLocator::scheduleSetupRepository()
{
    return m_scheduleSetupRepository.get();
}

ScheduleOfRatesRepository *RepositoryLocator::scheduleOfRatesRepository()
{
    return m_scheduleOfRatesRepository.get();
}

DraftEntityRepository *RepositoryLocator::draftEntityRepository()
{
    return m_draftEntityRepository.get();
}

IFCDetailRepository *RepositoryLocator::ifcDetailRepository()
{
    return m_ifcDetailRepository.get();
}

IFCWallRepository *RepositoryLocator::ifcWallRepository()
{
    return m_ifcWallRepository.get();
}

BillOfQuantityRepository *RepositoryLocator::billOfQuantityRepository()
{
    return m_billOfQuantityRepository.get();
}

BillOfQuantityLineRepository *RepositoryLocator::billOfQuantityLineRepository()
{
    return m_billOfQuantityLineRepository.get();
}

TaskRepository *RepositoryLocator::taskRepository()
{
    return m_taskRepository.get();
}

WorkOrderRepository *RepositoryLocator::workOrderRepository()
{
    return m_workOrderRepository.get();
}

ProjectBudgetRepository *RepositoryLocator::projectBudgetRepository()
{
    return m_projectBudgetRepository.get();
}

FileRepository *RepositoryLocator::fileRepository()
{
    return m_fileRepository.get();
}

PurchaseOrderRepository *RepositoryLocator::purchaseOrderRepository()
{
    return m_purchaseOrderRepository.get();
}

