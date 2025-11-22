#include "repositories/file_repository.h"

FileRepository::FileRepository(QObject* parent)
    : QObject(parent)
{}

std::unique_ptr<File> FileRepository::findById(int id)
{
    return nullptr;
}

std::vector<std::unique_ptr<File>> FileRepository::findAll()
{
    return {};
}

std::vector<File*> FileRepository::findAllQML()
{
    std::vector<File*> files;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM File")) {
        while (query.next()) {
            files.push_back(mapFromQueryQML(query, this));
        }
    }

    return files;
}

bool FileRepository::save(const File& entity)
{
    return false;
}

bool FileRepository::saveQML(File* entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool FileRepository::update(const File& entity)
{
    return false;
}

bool FileRepository::deleteById(int id)
{
    return false;
}

std::vector<std::unique_ptr<File>> FileRepository::findByApprovalStatus(bool status)
{
    return {};
}

QString FileRepository::getTableName() const
{
    return "File";
}

std::unique_ptr<File> FileRepository::mapFromQuery(const QSqlQuery& query) const
{
    return nullptr;
}

File* FileRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const
{
    auto file = new File(parent);

    file->setId(query.value("id").toInt());
    file->setGlobalId(query.value("global_id").toString());
    file->setApprovalStatus(query.value("approval_status").toBool());
    file->setDescription(query.value("description").toString());
    file->setFileUrl(query.value("file_url").toString());
    file->setFileLocalPath(query.value("file_local_path").toString());

    return file;
}

void FileRepository::bindEntityToQuery(QSqlQuery& query, const File& entity) const
{
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getDescription());
    query.addBindValue(entity.getFileUrl());
    query.addBindValue(entity.getFileLocalPath());
}

QString FileRepository::getInsertQuery() const
{
    return "INSERT INTO File (global_id, approval_status, description, file_url, file_local_path) "
           "VALUES (?, ?, ?, ?, ?)";
}

QString FileRepository::getUpdateQuery() const
{
    return "UPDATE File SET global_id = ?, approval_status = ?, description = ?, "
           "file_url = ?, file_local_path = ? WHERE id = ?";
}

