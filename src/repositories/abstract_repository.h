#ifndef ABSTRACT_REPOSITORY_H
#define ABSTRACT_REPOSITORY_H

#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <vector>
#include <memory>
#include "database/database_manager.h"

template<typename T>
class AbstractRepository
{
public:
    virtual ~AbstractRepository() = default;
    
    virtual std::unique_ptr<T> findById(int id) = 0;
    virtual std::vector<std::unique_ptr<T>> findAll() = 0;
    virtual bool save(const T& entity) = 0;
    virtual bool update(const T& entity) = 0;
    virtual bool deleteById(int id) = 0;
    virtual std::vector<std::unique_ptr<T>> findByApprovalStatus(bool status) = 0;

protected:
    std::shared_ptr<DatabaseManager> dbManager;
    
    AbstractRepository()
    {
        dbManager = DatabaseManager::getInstance();
    }
    virtual QString getTableName() const = 0;
    virtual std::unique_ptr<T> mapFromQuery(const QSqlQuery& query) const = 0;
    virtual void bindEntityToQuery(QSqlQuery& query, const T& entity) const = 0;
    virtual QString getInsertQuery() const = 0;
    virtual QString getUpdateQuery() const = 0;
};

#endif
