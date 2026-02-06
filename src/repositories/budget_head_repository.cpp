#include "repositories/budget_head_repository.h"
BudgetHeadRepository::BudgetHeadRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<BudgetHead> BudgetHeadRepository::findById(int id) {
    QSqlQuery query(dbManager->getDatabase());
    query.prepare("SELECT * FROM BudgetHead WHERE id = ?");
    query.addBindValue(id);

    if (query.exec() && query.next()) {
        return mapFromQuery(query);
    }

    return nullptr;
}

std::vector<std::unique_ptr<BudgetHead>> BudgetHeadRepository::findAll() {
    std::vector<std::unique_ptr<BudgetHead>> budgetHeads;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BudgetHead")) {
        while (query.next()) {
            budgetHeads.push_back(mapFromQuery(query));
        }
    }

    return budgetHeads;
}

std::vector<BudgetHead*> BudgetHeadRepository::findAllQML() {
    std::vector<BudgetHead*> budgetHeads;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM BudgetHead")) {
        while (query.next()) {
            budgetHeads.push_back(mapFromQueryQML(query, this));
        }
    }

    return budgetHeads;
}

bool BudgetHeadRepository::save(const BudgetHead& entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, entity);

    return query.exec();
}

bool BudgetHeadRepository::saveQML(BudgetHead* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}


bool BudgetHeadRepository::update(const BudgetHead& entity) {
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getUpdateQuery());
    bindEntityToQuery(query, entity);
    query.addBindValue(entity.getId());

    return query.exec();
}

bool BudgetHeadRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<BudgetHead>> BudgetHeadRepository::findByApprovalStatus(bool status) { return {}; }

std::vector<BudgetHead*> BudgetHeadRepository::findByApprovalStatusQML(bool status) { return {}; }

QString BudgetHeadRepository::getTableName() const { return "BudgetHead"; }

std::unique_ptr<BudgetHead> BudgetHeadRepository::mapFromQuery(const QSqlQuery& query) const {
    auto budgetHead = std::make_unique<BudgetHead>();
    budgetHead->setId(query.value("id").toInt());
    budgetHead->setGlobalId(query.value("global_id").toString());
    budgetHead->setApprovalStatus(query.value("approval_status").toString());
    budgetHead->setDescription(query.value("description").toString());

    return budgetHead;
}

BudgetHead* BudgetHeadRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto budgetHead = new BudgetHead(parent);
    budgetHead->setId(query.value("id").toInt());
    budgetHead->setGlobalId(query.value("global_id").toString());
    budgetHead->setApprovalStatus(query.value("approval_status").toString());
    budgetHead->setDescription(query.value("description").toString());

    return budgetHead;
}

void BudgetHeadRepository::bindEntityToQuery(QSqlQuery& query, const BudgetHead& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getDescription());
}
QString BudgetHeadRepository::getInsertQuery() const {
    return "INSERT INTO BudgetHead (global_id, approval_status, description) VALUES (?, ?, ?)";
}
QString BudgetHeadRepository::getUpdateQuery() const {
    return "UPDATE User SET global_id = ?, approval_status = ?, description = ? WHERE id = ?";
}
