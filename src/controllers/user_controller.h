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

signals:

private:
    UserRepository* m_userRepository;
};

#endif // USER_CONTROLLER_H
