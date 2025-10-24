#ifndef REPOSITORY_LOCATOR_H
#define REPOSITORY_LOCATOR_H

#include "repositories/user_repository.h"
#include "repositories/project_repository.h"
#include "repositories/bim_element_repository.h"
#include "repositories/bim_parameter_repository.h"

class RepositoryLocator
{
public:
    RepositoryLocator();

    static RepositoryLocator& instance();

    UserRepository* userRepository();
    ProjectRepository* projectRepository();
    BIMElementRepository* bimElementRepository();
    BIMParameterRepository* bimParameterRepository();

private:
    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<ProjectRepository> m_projectRepository;
    std::unique_ptr<BIMElementRepository> m_bimElementRepository;
    std::unique_ptr<BIMParameterRepository> m_bimParameterRepository;
};

#endif // REPOSITORY_LOCATOR_H
