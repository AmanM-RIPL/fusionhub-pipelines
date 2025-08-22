#ifndef REPOSITORY_LOCATOR_H
#define REPOSITORY_LOCATOR_H

#include "repositories/user_repository.h"

class RepositoryLocator
{
public:
    RepositoryLocator();

    static RepositoryLocator& instance();

    UserRepository* userRepository();

private:
    std::unique_ptr<UserRepository> m_userRepository;
};

#endif // REPOSITORY_LOCATOR_H
