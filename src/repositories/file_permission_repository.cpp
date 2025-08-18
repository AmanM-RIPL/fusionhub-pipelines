#include "repositories/file_permission_repository.h"
FilePermissionRepository::FilePermissionRepository() {}
std::unique_ptr<FilePermission> FilePermissionRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<FilePermission>> FilePermissionRepository::findAll() { return {}; }
bool FilePermissionRepository::save(const FilePermission& entity) { return false; }
bool FilePermissionRepository::update(const FilePermission& entity) { return false; }
bool FilePermissionRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<FilePermission>> FilePermissionRepository::findByApprovalStatus(bool status) { return {}; }
QString FilePermissionRepository::getTableName() const { return "FilePermission"; }
std::unique_ptr<FilePermission> FilePermissionRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void FilePermissionRepository::bindEntityToQuery(QSqlQuery& query, const FilePermission& entity) const {}
QString FilePermissionRepository::getInsertQuery() const { return ""; }
QString FilePermissionRepository::getUpdateQuery() const { return ""; }
