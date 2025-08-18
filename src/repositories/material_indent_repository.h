#ifndef MATERIAL_INDENT_REPOSITORY_H
#define MATERIAL_INDENT_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/material_indent.h"
#include <memory>
class MaterialIndentRepository : public AbstractRepository<MaterialIndent> {
public:
    MaterialIndentRepository();
    std::unique_ptr<MaterialIndent> findById(int id) override;
    std::vector<std::unique_ptr<MaterialIndent>> findAll() override;
    bool save(const MaterialIndent& entity) override;
    bool update(const MaterialIndent& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<MaterialIndent>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<MaterialIndent> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const MaterialIndent& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
