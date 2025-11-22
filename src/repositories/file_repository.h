#ifndef FILE_REPOSITORY_H
#define FILE_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/file.h"
#include <memory>

class FileRepository : public QObject, public AbstractRepository<File>
{
    Q_OBJECT

public:
    explicit FileRepository(QObject* parent = nullptr);

    // AbstractRepository interface
    Q_INVOKABLE std::unique_ptr<File> findById(int id) override;
    std::vector<std::unique_ptr<File>> findAll() override;

    // QML friendly
    Q_INVOKABLE std::vector<File*> findAllQML();

    bool save(const File& entity) override;
    Q_INVOKABLE bool saveQML(File* entity);

    bool update(const File& entity) override;
    bool deleteById(int id) override;

    std::vector<std::unique_ptr<File>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    std::unique_ptr<File> mapFromQuery(const QSqlQuery& query) const override;
    File* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& query, const File& entity) const override;

    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // FILE_REPOSITORY_H
