#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <memory>

class DatabaseManager
{
public:
    static std::shared_ptr<DatabaseManager> getInstance();
    
    bool initializeDatabase(const QString& projectName);
    bool createProjectFolder(const QString& projectName);
    QSqlDatabase getDatabase() const;
    bool executeQuery(const QString& queryString);
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    QString getProjectPath() const;
    
private:
    DatabaseManager() = default;
    static std::shared_ptr<DatabaseManager> instance;
    
    QSqlDatabase database;
    QString projectPath;
    QString databasePath;
    
    bool createTables();
    QString getCreateTableQuery(const QString& tableName);
};

#endif
