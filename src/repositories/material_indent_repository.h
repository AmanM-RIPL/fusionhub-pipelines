#ifndef MATERIAL_INDENT_REPOSITORY_H
#define MATERIAL_INDENT_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/material_indent.h"
#include <memory>
class MaterialIndentRepository : public QObject, public AbstractRepository<MaterialIndent> {
    Q_OBJECT
public:
    explicit MaterialIndentRepository(QObject* parent = nullptr);

    Q_INVOKABLE std::unique_ptr<MaterialIndent> findById(int id) override;

    std::vector<std::unique_ptr<MaterialIndent>> findAll() override;

    Q_INVOKABLE std::vector<MaterialIndent*> findAllQML();

    bool save(const MaterialIndent& entity) override;

    Q_INVOKABLE bool saveQML(MaterialIndent* entity);

    bool update(const MaterialIndent& entity) override;

    bool deleteById(int id) override;

    std::vector<std::unique_ptr<MaterialIndent>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;

    std::unique_ptr<MaterialIndent> mapFromQuery(const QSqlQuery& query) const override;

    MaterialIndent* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;

    void bindEntityToQuery(QSqlQuery& query, const MaterialIndent& entity) const override;

    QString getInsertQuery() const override;

    QString getUpdateQuery() const override;
};

#endif

