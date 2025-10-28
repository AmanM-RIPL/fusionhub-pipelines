#ifndef USER_REPOSITORY_H
#define USER_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/user.h"
#include <memory>

class UserRepository : public QObject, public AbstractRepository<User>
{
    Q_OBJECT

public:
    //UserRepository();
    explicit UserRepository(QObject* parent = nullptr);
    std::unique_ptr<User> findById(int id) override;
    std::vector<std::unique_ptr<User>> findAll() override;
    bool save(const User& entity) override;
    bool update(const User& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<User>> findByApprovalStatus(bool status) override;
    
    // Additional methods specific to User
    std::unique_ptr<User> findByUserId(const QString& userId);
    std::unique_ptr<User> findByUsername(const QString& username);

    std::unique_ptr<User> getUserDetailsByNameAndPassword(const QString& username, const QString& password);

    Q_INVOKABLE std::vector<User*> findAllQML();
    Q_INVOKABLE bool saveQML(User* entity);

protected:
    QString getTableName() const override;
    std::unique_ptr<User> mapFromQuery(const QSqlQuery& query) const override;
    User* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& query, const User& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

};

#endif // USER_REPOSITORY_H
