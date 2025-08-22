#include "user_controller.h"
#include "common/repository_locator.h"

UserController::UserController(QObject *parent)
    :
    QObject{parent},
    m_userRepository(RepositoryLocator::instance().userRepository())
{}

bool UserController::login(const QString &username, const QString &password) const
{
    qInfo() << username << " " << password;
    return true; // implement NetworkManager later
}
