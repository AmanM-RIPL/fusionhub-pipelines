#ifndef GOOD_RECEIVED_NOTE_REPOSITORY_H
#define GOOD_RECEIVED_NOTE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/good_received_note.h"
#include <memory>
class GoodReceivedNoteRepository : public AbstractRepository<GoodReceivedNote> {
public:
    GoodReceivedNoteRepository();
    std::unique_ptr<GoodReceivedNote> findById(int id) override;
    std::vector<std::unique_ptr<GoodReceivedNote>> findAll() override;
    bool save(const GoodReceivedNote& entity) override;
    bool update(const GoodReceivedNote& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<GoodReceivedNote>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<GoodReceivedNote> mapFromQuery(const QSqlQuery& query) const override;
    void bindEntityToQuery(QSqlQuery& query, const GoodReceivedNote& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
