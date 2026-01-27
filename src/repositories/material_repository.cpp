#include "repositories/material_repository.h"
MaterialRepository::MaterialRepository(QObject* parent) : QObject(parent) {}
std::unique_ptr<Material> MaterialRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<Material>> MaterialRepository::findAll() { return {}; }
std::vector<Material*> MaterialRepository::findAllQML() {
    std::vector<Material*> uom;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM Material")) {
        while (query.next()) {
            uom.push_back(mapFromQueryQML(query, this));
        }
    }

    return uom;
}
bool MaterialRepository::save(const Material& entity) { return false; }
bool MaterialRepository::saveQML(Material* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool MaterialRepository::update(const Material& entity) { return false; }
bool MaterialRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<Material>> MaterialRepository::findByApprovalStatus(bool status) { return {}; }
QString MaterialRepository::getTableName() const { return "Material"; }
std::unique_ptr<Material> MaterialRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

Material* MaterialRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto material = new Material(parent);
    material->setId(query.value("id").toInt());
    material->setGlobalId(query.value("global_id").toString());
    material->setApprovalStatus(query.value("approval_status").toString());
    material->setMaterialName(query.value("material_name").toString());
    material->setCategory(query.value("category").toString());
    material->setUnitOfMeasurementId(query.value("unit_of_measurement_id").toInt());

    return material;
}
void MaterialRepository::bindEntityToQuery(QSqlQuery& query, const Material& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getMaterialName());
    query.addBindValue(entity.getCategory());
    query.addBindValue(entity.getUnitOfMeasurementId());
}
QString MaterialRepository::getInsertQuery() const {
    return "INSERT INTO Material (global_id, approval_status, material_name, category, "
           "unit_of_measurement_id) "
           "VALUES (?, ?, ?, ?, ?)";
}
QString MaterialRepository::getUpdateQuery() const {
    return "UPDATE Material SET global_id = ?, approval_status = ?, material_name = ?, "
           "category = ?, unit_of_measurement_id = ? WHERE id = ?";
}
