#ifndef MATERIAL_REPOSITORY_H
#define MATERIAL_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/material.h"
#include <memory>
class MaterialRepository : public QObject, public AbstractRepository<Material> {
    Q_OBJECT
public:
    explicit MaterialRepository(QObject* parent = nullptr);
    Q_INVOKABLE std::unique_ptr<Material> findById(int id) override;
    std::vector<std::unique_ptr<Material>> findAll() override;
    Q_INVOKABLE std::vector<Material*> findAllQML();
    bool save(const Material& entity) override;
    Q_INVOKABLE bool saveQML(Material* entity);
    bool update(const Material& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<Material>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<Material> mapFromQuery(const QSqlQuery& query) const override;
    Material* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const Material& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
