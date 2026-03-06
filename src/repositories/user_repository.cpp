#include "repositories/user_repository.h"
#include <QDebug>

/*UserRepository::UserRepository()
{
}*/
UserRepository::UserRepository(QObject* parent) : QObject(parent) {}

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

std::unique_ptr<User> UserRepository::getUserDetailsByNameAndPassword(const QString& username, const QString& password)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM User WHERE user_name=? AND user_password=?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }

    return nullptr;
}

std::vector<std::unique_ptr<User>> UserRepository::findAll()
{
    std::vector<std::unique_ptr<User>> users;
    QSqlQuery query(dbManager->getDatabase());
    
    if (query.exec("SELECT user_id, user_name, user_fullname FROM User")) {
        while (query.next()) {
            users.push_back(mapFromQuery(query));
        }
    }
    
    return users;
}

std::vector<User*> UserRepository::findAllQML() {
    std::vector<User*> users;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM User")) {
        while (query.next()) {
            users.push_back(mapFromQueryQML(query, this));
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

bool UserRepository::saveQML(User* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);
    return query.exec();
}

bool UserRepository::update(const User& entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getUpdateQuery());
    //bindEntityToQuery(query, entity);
    query.addBindValue(entity.getId());
    query.addBindValue(entity.getUserFullName());
    query.addBindValue(entity.getUserMobile1());
    query.addBindValue(entity.getUserMobile2());
    query.addBindValue(entity.getUserEmail1());
    query.addBindValue(entity.getUserEmail2());
    query.addBindValue(entity.getUserJobTitle());
    query.addBindValue(entity.getUserStartDate());
    query.addBindValue(entity.getUserEndDate());
    query.addBindValue(entity.getUserPassword());
    query.addBindValue(entity.getUserId());

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
    user->setUserName(query.value("user_name").toString());
    user->setUserFullName(query.value("user_fullname").toString());
    user->setUserMobile1(query.value("user_mobile1").toString());
    user->setUserMobile2(query.value("user_mobile2").toString());
    user->setUserEmail1(query.value("user_email1").toString());
    user->setUserEmail2(query.value("user_email2").toString());
    user->setUserJobTitle(query.value("user_jobTitle").toString());
    user->setUserStartDate(query.value("user_startDate").toString());
    user->setUserEndDate(query.value("user_endDate").toString());
    user->setUserMonthlyDeskCostValue(query.value("user_monthlyDeskCostValue").toString());
    user->setUserPassword(query.value("user_password").toString());
    return user;
}

User* UserRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto user = new User(parent);
    user->setId(query.value("id").toInt());
    user->setGlobalId(query.value("global_id").toString());
    user->setApprovalStatus(query.value("approval_status").toBool());
    user->setUserId(query.value("user_id").toString());
    user->setUserFullName(query.value("user_fullname").toString());
    user->setUserName(query.value("user_name").toString());
    user->setUserMobile1(query.value("user_mobile1").toString());
    user->setUserMobile2(query.value("user_mobile2").toString());
    user->setUserEmail1(query.value("user_email1").toString());
    user->setUserEmail2(query.value("user_email2").toString());
    user->setUserJobTitle(query.value("user_jobTitle").toString());
    user->setUserStartDate(query.value("user_startDate").toString());
    user->setUserEndDate(query.value("user_endDate").toString());
    user->setUserMonthlyDeskCostValue(query.value("user_monthlyDeskCostValue").toString());
    user->setUserPassword(query.value("user_password").toString());

    return user;
}

void UserRepository::bindEntityToQuery(QSqlQuery& query, const User& entity) const
{
    query.addBindValue(entity.getId());
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getUserId());
    query.addBindValue(entity.getUserFullName());
    query.addBindValue(entity.getUserName());
    query.addBindValue(entity.getUserMobile1());
    query.addBindValue(entity.getUserMobile2());
    query.addBindValue(entity.getUserEmail1());
    query.addBindValue(entity.getUserEmail2());
    query.addBindValue(entity.getUserJobTitle());
    query.addBindValue(entity.getUserStartDate());
    query.addBindValue(entity.getUserEndDate());
    query.addBindValue(entity.getUserMonthlyDeskCostValue());
    query.addBindValue(entity.getUserPassword());
}

QString UserRepository::getInsertQuery() const
{
    return "INSERT INTO User (id, global_id, approval_status, user_id, user_fullname, user_name, "
           "user_mobile1, user_mobile2, user_email1, user_email2, user_jobTitle, user_startDate,"
           " user_endDate, user_monthlyDeskCostValue, user_password)"
           " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    /*
    return "INSERT INTO DraftEntity (tenant, createdOn, project, entity, createdByUser, "
           "entitySchema, changeHistory, nextApprovingUser,"
           "associatedApprovedEntity VALUES())";
    */
}

QString UserRepository::getUpdateQuery() const
{
     return "UPDATE User SET user_fullname = ?, user_mobile1 = ?, user_mobile2 = ?, user_email1 = ?,"
           "user_email2 = ?, user_jobTitle = ?, user_startDate = ?, user_endDate = ?, user_password = ? WHERE user_id = ?";
}
