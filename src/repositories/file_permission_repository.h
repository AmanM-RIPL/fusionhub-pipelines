#ifndef FILE_PERMISSION_REPOSITORY_H
#define FILE_PERMISSION_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/file_permission.h"
#include <memory>
class FilePermissionRepository : public AbstractRepository<FilePermission> {
public:
    FilePermissionRepository();
    std::unique_ptr<FilePermission> findById(int id) override;
    std::vector<std::unique_ptr<FilePermission>> findAll() override;
    bool save(const FilePermission& entity) override;
    bool update(const FilePermission& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<FilePermission>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<FilePermission> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const FilePermission& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
