#ifndef PROJECT_REPOSITORY_H
#define PROJECT_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include <memory>
#include "models/project.h"
//#include <QObject>
#include <string>

class ProjectRepository : public AbstractRepository<Project>
{
    //Q_OBJECT
public:
    //explicit ProjectRepository(QObject *parent = nullptr);
    ProjectRepository();

    std::unique_ptr<Project> findById(int id) override;
    std::vector<std::unique_ptr<Project>> findAll() override;
    bool save(const Project& entity) override;
    bool update(const Project& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<Project>> findByApprovalStatus(bool status) override;

    //Additional methods specific to User
   std::unique_ptr<Project> findByProjectId(const QString& projectId);
   std::unique_ptr<Project> findByProjectName(const QString& projectName);  
   void createJsonFileAndAppendJsonObject(const QString& filePath, const Project& entity);
   QString getProjectListAsJsonString(bool isBlocked);

protected:
    QString getTableName() const override;
    std::unique_ptr<Project> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const Project& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // PROJECT_REPOSITORY_H
