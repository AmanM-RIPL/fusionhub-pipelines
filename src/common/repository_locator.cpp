#include "repository_locator.h"

RepositoryLocator::RepositoryLocator() {
    m_userRepository = std::make_unique<UserRepository>();
    m_projectRepository = std::make_unique<ProjectRepository>();
    m_bimElementRepository = std::make_unique<BIMElementRepository>();
    m_bimParameterRepository = std::make_unique<BIMParameterRepository>();
    m_budgetheadRepository = std::make_unique<BudgetHeadRepository>();
    m_unitOfMeasurementRepository = std::make_unique<UnitOfMeasurementRepository>();
    m_vendorRepository =  std::make_unique<VendorRepository>();
    m_materialRepository = std::make_unique<MaterialRepository>();
    m_scheduleSetupRepository = std::make_unique<ScheduleSetupRepository>();
    m_draftEntityRepository = std::make_unique<DraftEntityRepository>();
    m_ifcDetailRepository = std::make_unique<IFCDetailRepository>();
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
BudgetHeadRepository *RepositoryLocator::budgetheadRepository()
{
    return m_budgetheadRepository.get();
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

DraftEntityRepository *RepositoryLocator::draftEntityRepository()
{
    return m_draftEntityRepository.get();
}

IFCDetailRepository *RepositoryLocator::ifcDetailRepository()
{
    return m_ifcDetailRepository.get();
}

