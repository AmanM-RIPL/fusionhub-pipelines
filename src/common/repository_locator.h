#ifndef REPOSITORY_LOCATOR_H
#define REPOSITORY_LOCATOR_H

#include "repositories/user_repository.h"
#include "repositories/project_repository.h"

class RepositoryLocator
{
public:
    RepositoryLocator();

    static RepositoryLocator& instance();

    UserRepository* userRepository();
    ProjectRepository* projectRepository();

private:
    std::unique_ptr<UserRepository> m_userRepository;
    std::unique_ptr<ProjectRepository> m_projectRepository;
};

#endif // REPOSITORY_LOCATOR_H
