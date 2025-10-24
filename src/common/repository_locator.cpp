#include "repository_locator.h"

RepositoryLocator::RepositoryLocator() {
    m_userRepository = std::make_unique<UserRepository>();
    m_projectRepository = std::make_unique<ProjectRepository>();
    m_bimElementRepository = std::make_unique<BIMElementRepository>();
    m_bimParameterRepository = std::make_unique<BIMParameterRepository>();
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
