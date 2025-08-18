#include "repositories/good_received_note_repository.h"
GoodReceivedNoteRepository::GoodReceivedNoteRepository() {}
std::unique_ptr<GoodReceivedNote> GoodReceivedNoteRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<GoodReceivedNote>> GoodReceivedNoteRepository::findAll() { return {}; }
bool GoodReceivedNoteRepository::save(const GoodReceivedNote& entity) { return false; }
bool GoodReceivedNoteRepository::update(const GoodReceivedNote& entity) { return false; }
bool GoodReceivedNoteRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<GoodReceivedNote>> GoodReceivedNoteRepository::findByApprovalStatus(bool status) { return {}; }
QString GoodReceivedNoteRepository::getTableName() const { return "GoodReceivedNote"; }
std::unique_ptr<GoodReceivedNote> GoodReceivedNoteRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
void GoodReceivedNoteRepository::bindEntityToQuery(QSqlQuery& query, const GoodReceivedNote& entity) const {}
QString GoodReceivedNoteRepository::getInsertQuery() const { return ""; }
QString GoodReceivedNoteRepository::getUpdateQuery() const { return ""; }
