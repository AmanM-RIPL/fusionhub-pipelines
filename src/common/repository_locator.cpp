#include "repository_locator.h"

RepositoryLocator::RepositoryLocator() {
    m_userRepository = std::make_unique<UserRepository>();
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
