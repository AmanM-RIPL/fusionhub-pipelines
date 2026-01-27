#include "repositories/material_indent_repository.h"

MaterialIndentRepository::MaterialIndentRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<MaterialIndent> MaterialIndentRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<MaterialIndent>> MaterialIndentRepository::findAll() { return {}; }

std::vector<MaterialIndent*> MaterialIndentRepository::findAllQML() {
    std::vector<MaterialIndent*> material;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM MaterialIndent")) {
        while (query.next()) {
            material.push_back(mapFromQueryQML(query, this));
        }
    }

    return material;
}
bool MaterialIndentRepository::save(const MaterialIndent& entity) { return false; }

bool MaterialIndentRepository::saveQML(MaterialIndent* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool MaterialIndentRepository::update(const MaterialIndent& entity) { return false; }

bool MaterialIndentRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<MaterialIndent>> MaterialIndentRepository::findByApprovalStatus(bool status) { return {}; }

QString MaterialIndentRepository::getTableName() const { return "MaterialIndent"; }

std::unique_ptr<MaterialIndent> MaterialIndentRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

MaterialIndent* MaterialIndentRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto materialIndent = new MaterialIndent(parent);
    materialIndent->setId(query.value("id").toInt());
    materialIndent->setGlobalId(query.value("global_id").toString());
    materialIndent->setApprovalStatus(query.value("approval_status").toBool());
    materialIndent->setQuantity(query.value("total_quantity").toDouble());
    materialIndent->setMaterialId(query.value("material_id").toInt());
    materialIndent->setTaskId(query.value("task_id").toInt());
    materialIndent->setMaterialName(query.value("material_name").toString());
    materialIndent->setTaskName(query.value("task_name").toString());
    return materialIndent;
}
void MaterialIndentRepository::bindEntityToQuery(QSqlQuery& query, const MaterialIndent& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getQuantity());
    query.addBindValue(entity.getMaterialId());
    query.addBindValue(entity.getTaskId());
}
QString MaterialIndentRepository::getInsertQuery() const {
    return "INSERT INTO MaterialIndent (global_id, approval_status, total_quantity, material_id, "
           "task_id) "
           "VALUES (?, ?, ?, ?, ?)";
}
QString MaterialIndentRepository::getUpdateQuery() const {
    return "UPDATE MaterialIndent SET global_id = ?, approval_status = ?, total_quantity = ?, "
           "material_id = ?, task_id = ? WHERE id = ?";
}

