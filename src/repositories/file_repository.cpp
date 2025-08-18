#include "repositories/file_repository.h"
FileRepository::FileRepository() {}
std::unique_ptr<File> FileRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<File>> FileRepository::findAll() { return {}; }
bool FileRepository::save(const File& entity) { return false; }
bool FileRepository::update(const File& entity) { return false; }
bool FileRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<File>> FileRepository::findByApprovalStatus(bool status) { return {}; }
QString FileRepository::getTableName() const { return "File"; }
std::unique_ptr<File> FileRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void FileRepository::bindEntityToQuery(QSqlQuery& query, const File& entity) const {}
QString FileRepository::getInsertQuery() const { return ""; }
QString FileRepository::getUpdateQuery() const { return ""; }
