#ifndef USER_CONTROLLER_H
#define USER_CONTROLLER_H

#include <QObject>
#include "repositories/user_repository.h"

class UserController : public QObject
{
    Q_OBJECT
public:
    explicit UserController(QObject *parent = nullptr);
    Q_INVOKABLE bool login(const QString& username, const QString& password) const;


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

    Q_INVOKABLE std::vector<User*> getUserList() const;


signals:

private:
    UserRepository* m_userRepository;
};

#endif // USER_CONTROLLER_H
