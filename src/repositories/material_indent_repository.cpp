#include "repositories/material_indent_repository.h"
MaterialIndentRepository::MaterialIndentRepository() {}
std::unique_ptr<MaterialIndent> MaterialIndentRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<MaterialIndent>> MaterialIndentRepository::findAll() { return {}; }
bool MaterialIndentRepository::save(const MaterialIndent& entity) { return false; }
bool MaterialIndentRepository::update(const MaterialIndent& entity) { return false; }
bool MaterialIndentRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<MaterialIndent>> MaterialIndentRepository::findByApprovalStatus(bool status) { return {}; }
QString MaterialIndentRepository::getTableName() const { return "MaterialIndent"; }
std::unique_ptr<MaterialIndent> MaterialIndentRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void MaterialIndentRepository::bindEntityToQuery(QSqlQuery& query, const MaterialIndent& entity) const {}
QString MaterialIndentRepository::getInsertQuery() const { return ""; }
QString MaterialIndentRepository::getUpdateQuery() const { return ""; }
