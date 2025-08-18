#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/file.h"
#include <memory>
class FileRepository : public AbstractRepository<File> {
public:
    FileRepository();
    std::unique_ptr<File> findById(int id) override;
    std::vector<std::unique_ptr<File>> findAll() override;
    bool save(const File& entity) override;
    bool update(const File& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<File>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<File> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const File& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
