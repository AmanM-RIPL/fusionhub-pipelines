#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/user.h"
#include <memory>

class UserRepository : public AbstractRepository<User>
{
public:
    UserRepository();
    
    std::unique_ptr<User> findById(int id) override;
    std::vector<std::unique_ptr<User>> findAll() override;
    bool save(const User& entity) override;
    bool update(const User& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<User>> findByApprovalStatus(bool status) override;
    
    // Additional methods specific to User
    std::unique_ptr<User> findByUserId(const QString& userId);
    std::unique_ptr<User> findByUsername(const QString& username);

protected:
    QString getTableName() const override;
    std::unique_ptr<User> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const User& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // USER_REPOSITORY_H
