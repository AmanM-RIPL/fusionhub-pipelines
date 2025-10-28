#include "bim_parameter_repository.h"

BIMParameterRepository::BIMParameterRepository(QObject *parent)
    : QObject{parent}
{}

std::unique_ptr<BIMParameter> BIMParameterRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BIMParameter>> BIMParameterRepository::findAll() { return {}; }
std::vector<BIMParameter*> BIMParameterRepository::findAllQML() {
    std::vector<BIMParameter*> uom;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BIMParameter")) {
        while (query.next()) {
            uom.push_back(mapFromQueryQML(query, this));
        }
    }

    return uom;
}

std::vector<BIMParameter*> BIMParameterRepository::findAllForElement(int id)
{
    std::vector<BIMParameter*> uom;
    QSqlQuery query(dbManager->getDatabase());

    query.prepare("SELECT * FROM BIMParameter WHERE bim_element_id = ?");
    query.addBindValue(id);

    if (query.exec()) {
        while (query.next()) {
            uom.push_back(mapFromQueryQML(query, this));
        }
    }

    return uom;
}
bool BIMParameterRepository::save(const BIMParameter& entity) { return false; }
bool BIMParameterRepository::saveQML(BIMParameter* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    bool result = query.exec();

    if (result)
    {
        if (query.next())
        {
            entity->setId(query.value("id").toInt());
        }
    }

    return result;
}
bool BIMParameterRepository::update(const BIMParameter& entity) { return false; }
bool BIMParameterRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BIMParameter>> BIMParameterRepository::findByApprovalStatus(bool status) { return {}; }
QString BIMParameterRepository::getTableName() const { return "BIMParameter"; }
std::unique_ptr<BIMParameter> BIMParameterRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

BIMParameter* BIMParameterRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto BimParameter = new BIMParameter(parent);
    BimParameter->setId(query.value("id").toInt());
    BimParameter->setGlobalId(query.value("global_id").toString());
    BimParameter->setApprovalStatus(query.value("approval_status").toBool());
    BimParameter->setKey(query.value("key").toString());
    BimParameter->setValue(query.value("value").toString());
    BimParameter->setBimElementId(query.value("bim_element_id").toInt());

    return BimParameter;
}
void BIMParameterRepository::bindEntityToQuery(QSqlQuery& query, const BIMParameter& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getKey());
    query.addBindValue(entity.getValue());
    query.addBindValue(entity.getBimElementId());
}
QString BIMParameterRepository::getInsertQuery() const {
    return "INSERT INTO BIMParameter (global_id, approval_status, key, value, "
           "bim_element_id) "
           "VALUES (?, ?, ?, ?, ?) RETURNING id";
}
QString BIMParameterRepository::getUpdateQuery() const {
    return "UPDATE BIMParameter SET global_id = ?, approval_status = ?, key = ?, "
           "value = ?, bim_element_id = ? WHERE id = ?";
}
