#include "user_controller.h"
#include "network/network_manager.h"
#include "common/repository_locator.h"
#include <QDateTime>
#include <QUuid>
#include <QDebug>

extern std::shared_ptr<User> gUser;

UserController::UserController(QObject *parent)
    : QObject(parent),
    m_userRepository(RepositoryLocator::instance().userRepository())
{
    // Connect NetworkManager signals to UserController slots
    NetworkManager* network = NetworkManager::getInstance();

    connect(network, &NetworkManager::loginDone,
            this, [this, network]() {
                QJsonObject userData = network->getLastUserData();
                onNetworkLoginSuccess(userData);
            });

    connect(network, &NetworkManager::loginFailed,
            this, &UserController::onNetworkLoginFailed);

}

void UserController::login(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        emit loginFailed("Username or password cannot be empty");
        return;
    }

    // Get NetworkManager instance and trigger async login
    NetworkManager* network = NetworkManager::getInstance();
    network->loginAPI(username, password);
}

void UserController::onNetworkLoginSuccess(const QJsonObject& userData)
{
    qDebug() << "UserController: Login successful";

    gUser->setUserId(userData["id"].toString());
    gUser->setUserName(userData["username"].toString());

    if (userData.contains("firstName")) {
        gUser->setUserFullName(userData["firstName"].toString());
    }
    if (userData.contains("email")) {
        gUser->setUserEmail1(userData["email"].toString());
    }
    if (userData.contains("mobile")) {
        gUser->setUserMobile1(userData["mobile"].toString());
    }

    // Emit success signal to QML
    emit loginSuccess(userData);
}

void UserController::onNetworkLoginFailed(const QString& error)
{
    qDebug() << "UserController: Login failed -" << error;
    qWarning() << "Login failed:" << error;

    // Emit failure signal to QML
    emit loginFailed(error);
}

std::shared_ptr<User> UserController::getCurrentUserObject() const
{
    return gUser;
}

int UserController::getCurrentId() const
{
    return gUser->getId();
}

QString UserController::getCurrentUserGlobalId() const
{
    return gUser->getGlobalId();
}

bool UserController::getCurrentUserApprovalStatus() const
{
    return gUser->getApprovalStatus();
}

QString UserController::getCurrentUserId() const
{
    return gUser->getUserId();
}

QString UserController::getCurrentUserFullName() const
{
    return gUser->getUserFullName();
}

QString UserController::getCurrentUserName() const
{
    return gUser->getUserName();
}

QString UserController::getCurrentUserMobile1() const
{
    return gUser->getUserMobile1();
}

QString UserController::getCurrentUserMobile2() const
{
    return gUser->getUserMobile2();
}

QString UserController::getCurrentUserEmail1() const
{
    return gUser->getUserEmail1();
}

QString UserController::getCurrentUserEmail2() const
{
    return gUser->getUserEmail2();
}

QString UserController::getCurrentUserJobTitle() const
{
    return gUser->getUserJobTitle();
}

QString UserController::getCurrentUserStartDate() const
{
    return gUser->getUserStartDate();
}

QString UserController::getCurrentUserEndDate() const
{
    return gUser->getUserEndDate();
}

QString UserController::getCurrentUserMonthlyDeskCostValue() const
{
    return gUser->getUserMonthlyDeskCostValue();
}

QString UserController::getCurrentUserPassword() const
{
    return gUser->getUserPassword();
}

void UserController::create(const QString& user_fullname,
                            const QString& user_name,
                            const QString& mobile1,
                            const QString& mobile2,
                            const QString& email1,
                            const QString& email2,
                            const QString& jobTitle,
                            const QString& startDate,
                            const QString& endDate,
                            const QString& monthlyDeskCostValue) const
{
    // This is used only for user creation
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
        return;
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    User user;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();

    QUuid uuid = QUuid::createUuid();
    QString randomUserId = uuid.toString();

    user.setId(id_in_milliseconds);
    user.setGlobalId("123");
    user.setApprovalStatus(true);
    user.setUserId(randomUserId);

    user.setUserFullName(user_fullname);
    user.setUserName(user_name);
    user.setUserMobile1(mobile1);
    user.setUserMobile2(mobile2);
    user.setUserEmail1(email1);
    user.setUserEmail2(email2);
    user.setUserJobTitle(jobTitle);
    user.setUserStartDate(startDate);
    user.setUserEndDate(endDate);
    user.setUserMonthlyDeskCostValue(monthlyDeskCostValue);
    user.setUserPassword("repl123");

    if(m_userRepository->saveQML(&user))
    {
        qDebug() << "Data Saved";
    }
    else
    {
        qDebug() << "Data not Saved";
    }
}

void UserController::update(const QString& userId,
                            const QString& user_fullname,
                            const QString& user_name,
                            const QString& mobile1,
                            const QString& mobile2,
                            const QString& email1,
                            const QString& email2,
                            const QString& jobTitle,
                            const QString& startDate,
                            const QString& endDate,
                            const QString& monthlyDeskCostValue,
                            const QString& password) const
{
    // This is used only for user update
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
        return;
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    User user;
    user.setUserId(userId);
    user.setUserFullName(user_fullname);
    user.setUserName(user_name);
    user.setUserMobile1(mobile1);
    user.setUserMobile2(mobile2);
    user.setUserEmail1(email1);
    user.setUserEmail2(email2);
    user.setUserJobTitle(jobTitle);
    user.setUserStartDate(startDate);
    user.setUserEndDate(endDate);
    user.setUserMonthlyDeskCostValue(monthlyDeskCostValue);
    user.setUserPassword(password);

    if(m_userRepository->update(user))
    {
        qDebug() << "Data Updated";
    }
    else
    {
        qDebug() << "Data not Updated";
    }
}

std::vector<User*> UserController::getUserList() const
{
    // This is used to fetch all users
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
        return std::vector<User*>();
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    return m_userRepository->findAllQML();
}

std::shared_ptr<User> UserController::getUserDetailsById(const QString& userid) const
{
    return m_userRepository->findByUserId(userid);
}
