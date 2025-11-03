#include "bim_element_repository.h"

BIMElementRepository::BIMElementRepository(QObject *parent)
    : QObject{parent}
{}

std::unique_ptr<BIMElement> BIMElementRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<BIMElement>> BIMElementRepository::findAll() { return {}; }
std::vector<BIMElement*> BIMElementRepository::findAllQML() {
    std::vector<BIMElement*> uom;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BIMElement")) {
        while (query.next()) {
            uom.push_back(mapFromQueryQML(query, this));
        }
    }

    return uom;
}
bool BIMElementRepository::save(const BIMElement& entity) { return false; }
bool BIMElementRepository::saveQML(BIMElement* entity) {

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
bool BIMElementRepository::update(const BIMElement& entity) { return false; }
bool BIMElementRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<BIMElement>> BIMElementRepository::findByApprovalStatus(bool status) { return {}; }
QString BIMElementRepository::getTableName() const { return "BIMElement"; }
std::unique_ptr<BIMElement> BIMElementRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

BIMElement* BIMElementRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto BimElement = new BIMElement(parent);
    BimElement->setId(query.value("id").toInt());
    BimElement->setGlobalId(query.value("global_id").toString());
    BimElement->setApprovalStatus(query.value("approval_status").toBool());
    BimElement->setName(query.value("name").toString());
    BimElement->setType(query.value("type").toString());
    BimElement->setLevel(query.value("level").toInt());

    return BimElement;
}
void BIMElementRepository::bindEntityToQuery(QSqlQuery& query, const BIMElement& entity) const {    
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getName());
    query.addBindValue(entity.getType());
    query.addBindValue(entity.getLevel());
}
QString BIMElementRepository::getInsertQuery() const {

     return "INSERT INTO BIMElement (global_id, approval_status, name, type, "
           "level) "
           "VALUES (?, ?, ?, ?, ?) RETURNING id";    
}
QString BIMElementRepository::getUpdateQuery() const {
    return "UPDATE BIMElement SET global_id = ?, approval_status = ?, name = ?, "
           "type = ?, level = ? WHERE id = ?";
}
