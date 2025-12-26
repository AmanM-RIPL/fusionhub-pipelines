#ifndef DRAFT_ENTITY_REPOSITORY_H
#define DRAFT_ENTITY_REPOSITORY_H

#include "repositories/abstract_repository.h"
#include "models/draft_entity.h"
#include <memory>
#include <QObject>

class DraftEntityRepository : public QObject, public AbstractRepository<DraftEntity>
{
    Q_OBJECT

public:
    explicit DraftEntityRepository(QObject *parent = nullptr);
    Q_INVOKABLE std::unique_ptr<DraftEntity> findById(int id) override;
    std::vector<std::unique_ptr<DraftEntity>> findAll() override;
    Q_INVOKABLE std::vector<DraftEntity*> findAllQML(const QString& entity);
    bool save(const DraftEntity& entity) override;
    Q_INVOKABLE bool saveQML(DraftEntity* entity);
    Q_INVOKABLE bool updateQML(DraftEntity* entity);
    bool update(const DraftEntity& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<DraftEntity>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<DraftEntity> mapFromQuery(const QSqlQuery& query) const override;
    DraftEntity* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const DraftEntity& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;

signals:
};

#endif // DRAFT_ENTITY_REPOSITORY_H
