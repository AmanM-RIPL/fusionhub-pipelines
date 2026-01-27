#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H
#include <QObject>
#include <QJsonObject>
#include "repositories/user_repository.h"

class UserController : public QObject
{
    Q_OBJECT
public:
    explicit UserController(QObject *parent = nullptr);
    Q_INVOKABLE bool login(const QString& username, const QString& password) const;
    Q_INVOKABLE void logout();
    //void initializeTimer();

    Q_INVOKABLE void login(const QString& username, const QString& password);

    Q_INVOKABLE void create(const QString& userFullName,
                            const QString& userName,
                            const QString& mobile1,
                            const QString& mobile2,
                            const QString& email1,
                            const QString& email2,
                            const QString& jobTitle,
                            const QString& startDate,
                            const QString& endDate,
                            const QString& monthlyDeskCostValue) const;
    Q_INVOKABLE void update(const QString& userId,
                            const QString& userFullName,
                            const QString& userName,
                            const QString& mobile1,
                            const QString& mobile2,
                            const QString& email1,
                            const QString& email2,
                            const QString& jobTitle,
                            const QString& startDate,
                            const QString& endDate,
                            const QString& monthlyDeskCostValue,
                            const QString& password) const;
    Q_INVOKABLE std::vector<User*> getUserList() const;
    Q_INVOKABLE std::shared_ptr<User> getUserDetailsById(const QString& userId) const;
    Q_INVOKABLE std::shared_ptr<User> getCurrentUserObject() const;
    Q_INVOKABLE int getCurrentId() const;
    Q_INVOKABLE QString getCurrentUserGlobalId() const;
    Q_INVOKABLE bool getCurrentUserApprovalStatus() const;
    Q_INVOKABLE QString getCurrentUserId() const;
    Q_INVOKABLE QString getCurrentUserFullName() const;
    Q_INVOKABLE QString getToken() const;
    Q_INVOKABLE QString getCurrentUserName() const;
    Q_INVOKABLE QString getCurrentUserMobile1() const;
    Q_INVOKABLE QString getCurrentUserMobile2() const;
    Q_INVOKABLE QString getCurrentUserEmail1() const;
    Q_INVOKABLE QString getCurrentUserEmail2() const;
    Q_INVOKABLE QString getCurrentUserJobTitle() const;
    Q_INVOKABLE QString getCurrentUserStartDate() const;
    Q_INVOKABLE QString getCurrentUserEndDate() const;
    Q_INVOKABLE QString getCurrentUserMonthlyDeskCostValue() const;
    Q_INVOKABLE QString getCurrentUserPassword() const;


    void startBackgroundSync();

signals:
    // Login signals
    void loginSuccess(const QJsonObject& userData);
    void loginFailed(const QString& errorMessage);

private slots:
    // Handle network manager responses
    void onNetworkLoginSuccess(const QJsonObject& userData);
    void onNetworkLoginFailed(const QString& error);

private:
    UserRepository* m_userRepository;
};

#endif // USER_CONTROLLER_H
