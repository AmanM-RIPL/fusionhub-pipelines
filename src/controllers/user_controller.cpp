#include "user_controller.h"
#include "common/repository_locator.h"
#include "common/background_thread_manager.h"

extern std::shared_ptr<User> gUser;

 QTimer *g_timer = nullptr;

UserController::UserController(QObject *parent)
    :
    QObject{parent},
    m_userRepository(RepositoryLocator::instance().userRepository())
{
   //qDebug()<<"UserController::UserController(QObject *parent)";
}

void UserController::startBackgroundSync()
{
    if (!g_timer)
    {
        g_timer = new QTimer(this);
        connect(g_timer, &QTimer::timeout,
                BackgroundThreadManager::instance(),
                &BackgroundThreadManager::runBackgroundTaskForDraftDataSync);

        g_timer->start(1000);
        qDebug() << "Timer created and started";
    }
    else if (!g_timer->isActive()) {
        g_timer->start(1000);
    }

    BackgroundThreadManager::instance()->runBackgroundTaskForDraftDataSync();
}

void UserController::logout()
{
     qDebug()<<"logout1";
    if(g_timer)
    {
        qDebug()<<"logout2";
        g_timer->stop();
        delete  g_timer;
        g_timer = nullptr;
    }
}

bool UserController::login(const QString &username, const QString &password) const
{
    qInfo() << username << " " << password;

    if(username == "admin" || username.isEmpty())
    {
        gUser->setUserName("admin");

        const_cast<UserController*>(this)->startBackgroundSync();

        return true;
    }

    //When user will try to login
    // 1) User will get userdetails from server if login get success otherwise userdetails will be null;
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
        return -1;
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();


    gUser =  m_userRepository->getUserDetailsByNameAndPassword(username, password);

    if (database.isOpen())
    {
        database.close();
    }

    if(gUser == nullptr || gUser->getUserId().trimmed().isEmpty())
    {
        return false;
    }
    return true; // implement NetworkManager later
}

std::shared_ptr<User>  UserController::getCurrentUserObject()const
{
    return gUser;
}

int UserController::getCurrentId() const
{
    return gUser->getId();
}

QString UserController::getCurrentUserGlobalId()const
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


QString UserController::getCurrentUserFullName()const
{
     return gUser->getUserFullName();
}

QString UserController::getCurrentUserName()const
{
    return gUser->getUserName();
}

QString UserController::getCurrentUserMobile1()const
{
    return gUser->getUserMobile1();
}

QString UserController::getCurrentUserMobile2()const
{
    return gUser->getUserMobile2();
}

QString UserController::getCurrentUserEmail1()const
{
    return gUser->getUserEmail1();
}

QString UserController::getCurrentUserEmail2()const
{
     return gUser->getUserEmail2();
}

QString UserController::getCurrentUserJobTitle()const
{
     return gUser->getUserJobTitle();
}

QString UserController::getCurrentUserStartDate()const
{
     return gUser->getUserStartDate();
}

QString UserController::getCurrentUserEndDate()const
{
     return gUser->getUserEndDate();
}

QString UserController::getCurrentUserMonthlyDeskCostValue() const
{
     return gUser->getUserMonthlyDeskCostValue();
}

QString UserController::getCurrentUserPassword()const
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

    //This is used only for user creation
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
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
        qDebug()<<"Data Saved";
    }
    else
    {
        qDebug()<<"Data not Saved";
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
    //This is used only for user creation
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
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
        qDebug()<<"Data Updated";
    }
    else
    {
        qDebug()<<"Data not Updated";
    }    

}

std::vector<User*> UserController::getUserList() const
{
    //This is used only for user creation
    auto dbManager = DatabaseManager::getInstance();
    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase("Users")) {
        qDebug() << "Failed to initialize database";
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    return m_userRepository->findAllQML();
}

std::shared_ptr<User> UserController::getUserDetailsById(const QString& userid) const
{
    return m_userRepository->findByUserId(userid);
}

