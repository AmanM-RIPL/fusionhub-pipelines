#include "user_controller.h"
#include "network/network_manager.h"
#include "common/repository_locator.h"
#include "common/background_thread_manager.h"
#include <QDateTime>
#include <QUuid>
#include <QDebug>

extern std::shared_ptr<User> gUser;

QTimer *g_timer = nullptr;

UserController::UserController(QObject *parent)
    : QObject(parent),
    m_userRepository(RepositoryLocator::instance().userRepository())
{
    NetworkManager* network = NetworkManager::getInstance();

    connect(network, &NetworkManager::loginDone,
            this, [this, network]() {
                QJsonObject userData = network->getLastUserData();
                onNetworkLoginSuccess(userData);
            });

    connect(network, &NetworkManager::loginFailed,
            this, &UserController::onNetworkLoginFailed);
}

void UserController::startBackgroundSync()
{
    // if (g_timer)
    //     return;

    // qDebug() << "Creating background sync timer";

    // g_timer = new QTimer(this);
    // g_timer->setInterval(4000);

    // connect(g_timer, &QTimer::timeout,
    //         BackgroundThreadManager::instance(),
    //         &BackgroundThreadManager::runBackgroundTaskForDraftDataSync);

    // g_timer->start();
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

bool UserController::login(const QString& username, const QString& password)
{
    if(username == "admin" || username.isEmpty())
    {
        gUser->setUserName("admin");

       // const_cast<UserController*>(this)->startBackgroundSync();
    }
    else if (username.isEmpty() || password.isEmpty()) {
        emit loginFailed("Username or password cannot be empty");
        return false;
    }

    // // Get NetworkManager instance and trigger async login
    // NetworkManager* network = NetworkManager::getInstance();
    // network->loginAPI(username, password);

    QJsonObject userData;
    userData["id"] = 2;
    userData["username"] = "admin";
    userData["token"] = "44rrfdcsxx";
    userData["firstName"] = "First Name";
    userData["lastName"] = "Last Name";
    userData["email"] = "sample@gmail.com";
    userData["mobile"] = "+91-990543";

    onNetworkLoginSuccess(userData);

    return true;
}

void UserController::onNetworkLoginSuccess(const QJsonObject& userData)
{
    gUser->setId(userData["id"].toInt());
    gUser->setUserName(userData["username"].toString());
    gUser->setToken(userData["token"].toString());

    if (userData.contains("firstName")) {
        gUser->setUserFullName(userData["firstName"].toString());
    }
    if (userData.contains("email")) {
        gUser->setUserEmail1(userData["email"].toString());
    }
    if (userData.contains("mobile")) {
        gUser->setUserMobile1(userData["mobile"].toString());
    }
    startBackgroundSync();
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

QString UserController::getToken() const
{
    return gUser->getToken();
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

void UserController::createUser(const QString& username, const QString& password, const QString& email,
                                const QString& firstName,
                                const QString& lastName,
                                const QString& mobile)
{
    QString token = NetworkManager::getInstance()->getStoredToken();

    if (token.isEmpty()) {
        emit userCreationFailed("Authorization token missing. Please login first");
        return;
    }

    QUrl url("http://127.0.0.1:8080/api/v1/default/user");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // Authorization Header Set
    request.setRawHeader("Authorization",
                         QString("Bearer %1").arg(token).toUtf8());

    QJsonObject json;
    json["username"] = username;
    json["password"] = password;
    json["email"] = email;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["mobile"] = mobile;

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();
    QNetworkAccessManager* m_manager = new QNetworkAccessManager(NetworkManager::getInstance());
    qDebug() << "data=" << data;
    QNetworkReply *reply = m_manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {

        QByteArray responseData = reply->readAll();
        // emit userCreated(responseData);
        qDebug() << "userCreated: " << responseData;
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Network error:" << reply->errorString();
            reply->deleteLater();
            qDebug() << "userCreatedFailed: " << responseData;
            //  return ;
        }

        reply->deleteLater();
    });
}


void UserController::updateUser(const QString& userId,
                                const QString& username,
                                const QString& firstName,
                                const QString& lastName,
                                const QString& email,
                                const QString& mobile,
                                const QString& password)
{
    QString token = NetworkManager::getInstance()->getStoredToken();

    if (token.isEmpty()) {
        emit userListFailed("Authorization token missing. Please login first");
        return;
    }

    QUrl url(QString("http://127.0.0.1:8080/api/v1/default/user/%1").arg(userId));
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QJsonObject json;
    json["username"] = username;
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["email"] = email;
    json["mobile"] = mobile;
    if(!password.isEmpty()) {
        json["password"] = password;
    }

    QJsonDocument doc(json);
    QByteArray data = doc.toJson();

    QNetworkAccessManager* m_manager = new QNetworkAccessManager(NetworkManager::getInstance());

    QNetworkReply *reply = m_manager->put(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray responseData = reply->readAll();

        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Update Success:" << responseData;
            getUserList();
        } else {
            qWarning() << "Update Failed:" << reply->errorString();
            qDebug() << "Server Response:" << responseData;
        }

        reply->deleteLater();
    });
}


void UserController::getUserList()
{
    QString token = NetworkManager::getInstance()->getStoredToken();
    if(token.isEmpty()) {
        emit userListFailed("Please login first");
        return;
    }

    QUrl url("http://127.0.0.1:8080/api/v1/default/user");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);

            if(doc.isArray()) {
                QJsonArray array = doc.array();
                QVariantList list;
                for(const QJsonValue &value : array) {
                    QJsonObject obj = value.toObject();
                    QVariantMap user;
                    user["id"] = obj["id"].toInt();
                    user["username"] = obj["username"].toString();
                    user["firstName"] = obj["firstName"].toString();
                    user["lastName"] = obj["lastName"].toString();
                    user["email"] = obj["email"].toString();
                    user["mobile"] = obj["mobile"].toString();

                    list.append(user);
                }
                // qDebug() << "Emitting list with count:" << list.count();
                emit userListReceived(list);
            } else {
                emit userListFailed("Invalid JSON format");
            }
        } else {
            emit userListFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void UserController::getUserById(int id)
{
    QString token = NetworkManager::getInstance()->getStoredToken();
    if(token.isEmpty()) {
        emit userDetailsFailed("Auth token missing");
        return;
    }

    QUrl url(QString("http://127.0.0.1:8080/api/v1/default/user/%1").arg(id));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);

            if(doc.isObject()) {
                QJsonObject obj = doc.object();
                QVariantMap details;

                details["id"] = obj["id"].toInt();
                details["firstName"] = obj["first_name"].toString();
                details["lastName"] = obj["last_name"].toString();
                details["email"] = obj["email"].toString();
                details["mobile"] = obj["mobile"].toString();

                emit userDetailsReceived(details);
            } else {
                emit userDetailsFailed("Invalid user data format");
            }
        } else {
            emit userDetailsFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

// void UserController::create(const QString& user_fullname,
//                             const QString& user_name,
//                             const QString& mobile1,
//                             const QString& mobile2,
//                             const QString& email1,
//                             const QString& email2,
//                             const QString& jobTitle,
//                             const QString& startDate,
//                             const QString& endDate,
//                             const QString& monthlyDeskCostValue) const
// {
//     // This is used only for user creation
//     auto dbManager = DatabaseManager::getInstance();
//     QSqlDatabase database = dbManager->getDatabase();
//     if (database.isOpen())
//     {
//         database.close();
//     }

//     if (!dbManager->initializeDatabase("Users")) {
//         qDebug() << "Failed to initialize database";
//         return;
//     }

//     qDebug() << "Database initialized successfully!";
//     qDebug() << "Project path:" << dbManager->getProjectPath();

//     User user;

//     qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();

//     QUuid uuid = QUuid::createUuid();
//     QString randomUserId = uuid.toString();

//     user.setId(id_in_milliseconds);
//     user.setGlobalId("123");
//     user.setApprovalStatus(true);
//     user.setUserId(randomUserId);

//     user.setUserFullName(user_fullname);
//     user.setUserName(user_name);
//     user.setUserMobile1(mobile1);
//     user.setUserMobile2(mobile2);
//     user.setUserEmail1(email1);
//     user.setUserEmail2(email2);
//     user.setUserJobTitle(jobTitle);
//     user.setUserStartDate(startDate);
//     user.setUserEndDate(endDate);
//     user.setUserMonthlyDeskCostValue(monthlyDeskCostValue);
//     user.setUserPassword("repl123");

//     if(m_userRepository->saveQML(&user))
//     {
//         qDebug() << "Data Saved";
//     }
//     else
//     {
//         qDebug() << "Data not Saved";
//     }
// }

void UserController::update(const QString& userId,const QString& user_fullname,const QString& user_name, const QString& mobile1,const QString& mobile2, const QString& email1, const QString& email2,const QString& jobTitle,
                            const QString& startDate, const QString& endDate, const QString& monthlyDeskCostValue,
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

std::shared_ptr<User> UserController::getUserDetailsById(const QString& userid) const
{
    return m_userRepository->findByUserId(userid);
}
