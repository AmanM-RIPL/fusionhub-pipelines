#include "repositories/good_received_note_repository.h"
\
GoodReceivedNoteRepository::GoodReceivedNoteRepository(QObject* parent) : QObject(parent) {}
std::unique_ptr<GoodReceivedNote> GoodReceivedNoteRepository::findById(int id) { return nullptr; }

std::vector<std::unique_ptr<GoodReceivedNote>> GoodReceivedNoteRepository::findAll() { return {}; }

std::vector<GoodReceivedNote*> GoodReceivedNoteRepository::findAllQML() {
    std::vector<GoodReceivedNote*> purchaseOrderLine;
    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM GoodReceivedNote")) {
        while (query.next()) {
            purchaseOrderLine.push_back(mapFromQueryQML(query, this));
        }
    }

    return purchaseOrderLine;
}
bool GoodReceivedNoteRepository::save(const GoodReceivedNote& entity) { return false; }
bool GoodReceivedNoteRepository::saveQML(GoodReceivedNote* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}

bool GoodReceivedNoteRepository::update(const GoodReceivedNote& entity) { return false; }

bool GoodReceivedNoteRepository::deleteById(int id) { return false; }

std::vector<std::unique_ptr<GoodReceivedNote>> GoodReceivedNoteRepository::findByApprovalStatus(bool status) { return {}; }

QString GoodReceivedNoteRepository::getTableName() const { return "GoodReceivedNote"; }

std::unique_ptr<GoodReceivedNote> GoodReceivedNoteRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

GoodReceivedNote* GoodReceivedNoteRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto goodReceivedNote = new GoodReceivedNote(parent);
    goodReceivedNote->setId(query.value("id").toInt());
    goodReceivedNote->setGlobalId(query.value("global_id").toString());
    goodReceivedNote->setApprovalStatus(query.value("approval_status").toString());
    goodReceivedNote->setQuantity(query.value("amount_of_material_received").toInt());
    goodReceivedNote->setPurchaseOrderLineId(query.value("purchase_order_line_id").toInt());

    return goodReceivedNote;
}
void GoodReceivedNoteRepository::bindEntityToQuery(QSqlQuery& query, const GoodReceivedNote& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getQuantity());
    query.addBindValue(entity.getPurchaseOrderLineId());
}
QString GoodReceivedNoteRepository::getInsertQuery() const {
    return "INSERT INTO GoodReceivedNote (global_id, approval_status, amount_of_material_received, "
           "purchase_order_line_id) "
           "VALUES (?, ?, ?, ?)";
}
QString GoodReceivedNoteRepository::getUpdateQuery() const {
    return "UPDATE GoodReceivedNote SET global_id = ?, approval_status = ?, amount_of_material_received = ?, "
           " purchase_order_line_id = ? WHERE id = ?";
}
