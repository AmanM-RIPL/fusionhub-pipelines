#ifndef REPOSITORY_LOCATOR_H
#define REPOSITORY_LOCATOR_H

#include "repositories/user_repository.h"
#include "repositories/project_repository.h"
#include "repositories/bim_element_repository.h"
#include "repositories/bim_parameter_repository.h"
#include "repositories/budget_head_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/material_repository.h"
#include "repositories/unit_of_measurement_repository.h"
#include "repositories/schedule_setup_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/ifc_detail_repository.h"


class RepositoryLocator
{
public:
    RepositoryLocator();

    static RepositoryLocator& instance();

    UserRepository* userRepository();
    ProjectRepository* projectRepository();
    BIMElementRepository* bimElementRepository();
    BIMParameterRepository* bimParameterRepository();
    BudgetHeadRepository* budgetheadRepository();
    VendorRepository* vendorRepository();
    MaterialRepository* materialRepository();
    UnitOfMeasurementRepository* unitOfMeasurementRepository();
    ScheduleSetupRepository* scheduleSetupRepository();
    DraftEntityRepository* draftEntityRepository();
    IFCDetailRepository* ifcDetailRepository();


private:
    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<ProjectRepository> m_projectRepository;
    std::unique_ptr<BIMElementRepository> m_bimElementRepository;
    std::unique_ptr<BIMParameterRepository> m_bimParameterRepository;
    std::unique_ptr<BudgetHeadRepository> m_budgetheadRepository;
    std::unique_ptr<VendorRepository> m_vendorRepository;
    std::unique_ptr<MaterialRepository> m_materialRepository;
    std::unique_ptr<UnitOfMeasurementRepository> m_unitOfMeasurementRepository;
    std::unique_ptr<ScheduleSetupRepository> m_scheduleSetupRepository;
    std::unique_ptr<DraftEntityRepository> m_draftEntityRepository;
    std::unique_ptr<IFCDetailRepository> m_ifcDetailRepository;

};

#endif // REPOSITORY_LOCATOR_H
