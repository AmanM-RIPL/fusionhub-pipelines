#include "repository_locator.h"

RepositoryLocator::RepositoryLocator() {
    m_userRepository = std::make_unique<UserRepository>();
    m_projectRepository = std::make_unique<ProjectRepository>();
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
