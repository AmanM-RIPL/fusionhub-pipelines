#include "repositories/user_repository.h"
#include <QDebug>

UserRepository::UserRepository()
{
}

std::unique_ptr<User> UserRepository::findById(int id)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM User WHERE id = ?");
    query.addBindValue(id);
    
    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    
    return nullptr;
}

std::vector<std::unique_ptr<User>> UserRepository::findAll()
{
    std::vector<std::unique_ptr<User>> users;
    QSqlQuery query(dbManager->getDatabase());
    
    if (query.exec("SELECT * FROM User")) {
        while (query.next()) {
            users.push_back(mapFromQuery(query));
        }
    }
    
    return users;
}

bool UserRepository::save(const User& entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, entity);
    
    return query.exec();
}

bool UserRepository::update(const User& entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getUpdateQuery());
    bindEntityToQuery(query, entity);
    query.addBindValue(entity.getId());
    
    return query.exec();
}

bool UserRepository::deleteById(int id)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("DELETE FROM User WHERE id = ?");
    query.addBindValue(id);
    
    return query.exec();
}

std::vector<std::unique_ptr<User>> UserRepository::findByApprovalStatus(bool status)
{
    std::vector<std::unique_ptr<User>> users;
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM User WHERE approval_status = ?");
    query.addBindValue(status);
    
    if (query.exec()) {
        while (query.next()) {
            users.push_back(mapFromQuery(query));
        }
    }
    
    return users;
}

std::unique_ptr<User> UserRepository::findByUserId(const QString& userId)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM User WHERE user_id = ?");
    query.addBindValue(userId);
    
    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    
    return nullptr;
}

std::unique_ptr<User> UserRepository::findByUsername(const QString& username)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM User WHERE username = ?");
    query.addBindValue(username);
    
    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }
    
    return nullptr;
}

QString UserRepository::getTableName() const
{
    return "User";
}

std::unique_ptr<User> UserRepository::mapFromQuery(const QSqlQuery& query) const
{
    auto user = std::make_unique<User>();
    user->setId(query.value("id").toInt());
    user->setGlobalId(query.value("global_id").toString());
    user->setApprovalStatus(query.value("approval_status").toBool());
    user->setUserId(query.value("user_id").toString());
    user->setUsername(query.value("username").toString());
    
    return user;
}

void UserRepository::bindEntityToQuery(QSqlQuery& query, const User& entity) const
{
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getUserId());
    query.addBindValue(entity.getUsername());
}

QString UserRepository::getInsertQuery() const
{
    return "INSERT INTO User (global_id, approval_status, user_id, username) VALUES (?, ?, ?, ?)";
}

QString UserRepository::getUpdateQuery() const
{
    return "UPDATE User SET global_id = ?, approval_status = ?, user_id = ?, username = ? WHERE id = ?";
}
