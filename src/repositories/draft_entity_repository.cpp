#include "draft_entity_repository.h"

DraftEntityRepository::DraftEntityRepository(QObject *parent)
    : QObject(parent)
{

}

std::unique_ptr<DraftEntity> DraftEntityRepository::findById(int id)
{
    return nullptr;
}
std::vector<std::unique_ptr<DraftEntity>> DraftEntityRepository::findAll()
{
    return {};
}
std::vector<DraftEntity*> DraftEntityRepository::findAllQML(const QString& entity) {
    std::vector<DraftEntity*> draftEntitys;

    QSqlQuery query(dbManager->getDatabase());
    QString queryString = QString("SELECT * FROM DraftEntity WHERE entity='%1'").arg(entity);


    if (query.exec(queryString)) {
        while (query.next()) {
            draftEntitys.push_back(mapFromQueryQML(query, this));
        }
    }
    return draftEntitys;
}

std::vector<DraftEntity*> DraftEntityRepository::findAllApprovedQML() {
    std::vector<DraftEntity*> approvedDraftEntitys;

    QSqlQuery query(dbManager->getDatabase());

    QString queryString = "SELECT * FROM DraftEntity WHERE approvalStatus = 'Approved'";
    qDebug() << "Query String:" << queryString;

    if (query.exec(queryString)) {
        while (query.next()) {
            approvedDraftEntitys.push_back(mapFromQueryQML(query, this));
        }
        qDebug() << "Total records fetched:" << approvedDraftEntitys.size();
    } else {
        qDebug() << "Error fetching approved draft entities:" << query.lastError().text();
    }
    return approvedDraftEntitys;
}

bool DraftEntityRepository::deleteAllApprovedQML()
{
    QSqlQuery query(dbManager->getDatabase());

    QString queryString = "DELETE FROM DraftEntity WHERE approvalStatus = 'Approved'";
    qDebug() << "Delete Query:" << queryString;

    if (query.exec(queryString)) {
        qDebug() << "Approved DraftEntity deleted. Rows affected:"
                 << query.numRowsAffected();
        return true;
    } else {
        qDebug() << "Error deleting approved draft entities:"
                 << query.lastError().text();
        return false;
    }
}


bool DraftEntityRepository::save(const DraftEntity& entity) { return false; }

bool DraftEntityRepository::saveQML(DraftEntity* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);
    //return query.exec();

    //Execute the query
    if (!query.exec()) {
        qDebug() << "Error inserting Data:" << query.lastError().text();
        return 0;
    } else {
        qDebug() << "Data inserted successfully!";
        return 1;
    }

    return 1;
}

bool DraftEntityRepository::updateQML(DraftEntity* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getUpdateQuery());
    query.addBindValue(entity->getEntitySchema());
    query.addBindValue(entity->getId());

    if (!query.exec()) {
        qDebug() << "Error Updated Data:" << query.lastError().text();
        return 0;
    } else {
        qDebug() << "Data Updated successfully!";
        return 1;
    }

    return 1;
}

bool DraftEntityRepository::approveQML(DraftEntity* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getApproveUpdateQuery());
    query.addBindValue(entity->getApprovalStatus());
    query.addBindValue(entity->getNextApprovingUser());
    query.addBindValue(entity->getId());

    if (!query.exec()) {
        qDebug() << "Error Updated Data:" << query.lastError().text();
        return 0;
    } else {
        qDebug() << "Data Updated successfully!";
        return 1;
    }

    return 1;
}

bool DraftEntityRepository::cancelQML(DraftEntity* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getCancelQuery());
    query.addBindValue(entity->getApprovalStatus());
    query.addBindValue(entity->getNextApprovingUser());
    query.addBindValue(entity->getId());

    if (!query.exec()) {
        qDebug() << "Error Updated Data:" << query.lastError().text();
        return 0;
    } else {
        qDebug() << "Data Updated successfully!";
        return 1;
    }

    return 1;
}


bool DraftEntityRepository::update(const DraftEntity& entity) { return false; }

bool DraftEntityRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<DraftEntity>> DraftEntityRepository::findByApprovalStatus(bool status) { return {}; }
QString DraftEntityRepository::getTableName() const { return "DraftEntity"; }
std::unique_ptr<DraftEntity> DraftEntityRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
DraftEntity* DraftEntityRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto draftEntity = new DraftEntity(parent);

    draftEntity->setId(query.value("id").toInt());
    draftEntity->setTenant(query.value("tenant").toInt());
    draftEntity->setProject(query.value("project").toInt());
    draftEntity->setEntity(query.value("entity").toString());
    draftEntity->setCreatedOn(query.value("createdOn").toDate());
    draftEntity->setCreatedByUser(query.value("createdByUser").toInt());
    draftEntity->setApprovalStatus(query.value("approvalStatus").toString());
    draftEntity->setGlobalId("");
    draftEntity->setEntitySchema(query.value("entitySchema").toString());
    draftEntity->setNextApprovingUser(query.value("nextApprovingUser").toInt());
    draftEntity->setAssociatedApprovedEntity(query.value("associatedApprovedEntity").toInt());
    draftEntity->setChangeHistory(query.value("changeHistory").toString());

    return draftEntity;
}
void DraftEntityRepository::bindEntityToQuery(QSqlQuery& query, const DraftEntity& entity) const {

    //query.addBindValue(entity.getId());
    query.addBindValue(entity.getTenant());
    query.addBindValue(entity.getProject());
    query.addBindValue(entity.getEntity());
    query.addBindValue(entity.getCreatedOn());
    query.addBindValue(entity.getCreatedByUser());
    query.addBindValue(entity.getEntitySchema());
    query.addBindValue(entity.getNextApprovingUser());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getAssociatedApprovedEntity());
    query.addBindValue(entity.getChangeHistory());

}

QString DraftEntityRepository::getInsertQuery() const {
    return "INSERT INTO DraftEntity (tenant, project, entity, createdOn, createdByUser, entitySchema, nextApprovingUser, approvalStatus, associatedApprovedEntity, changeHistory) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
}
QString DraftEntityRepository::getUpdateQuery() const {
    return "UPDATE DraftEntity SET  entitySchema = ? WHERE id = ?";
}
QString DraftEntityRepository::getApproveUpdateQuery() const {
    return "UPDATE DraftEntity SET  approvalStatus = ?, nextApprovingUser = ? WHERE id = ?";
}

QString DraftEntityRepository::getCancelQuery() const {
    return "UPDATE DraftEntity SET  approvalStatus = ?, nextApprovingUser = ? WHERE id = ?";
}
