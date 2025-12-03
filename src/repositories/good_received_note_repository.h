#ifndef GOOD_RECEIVED_NOTE_REPOSITORY_H
#define GOOD_RECEIVED_NOTE_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/good_received_note.h"
#include <memory>
class GoodReceivedNoteRepository : public QObject, public AbstractRepository<GoodReceivedNote> {
    Q_OBJECT
public:
    explicit GoodReceivedNoteRepository(QObject* parent = nullptr);
    Q_INVOKABLE std::unique_ptr<GoodReceivedNote> findById(int id) override;
    std::vector<std::unique_ptr<GoodReceivedNote>> findAll() override;
    Q_INVOKABLE std::vector<GoodReceivedNote*> findAllQML();
    bool save(const GoodReceivedNote& entity) override;
    Q_INVOKABLE bool saveQML(GoodReceivedNote* entity);
    bool update(const GoodReceivedNote& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<GoodReceivedNote>> findByApprovalStatus(bool status) override;

protected:
    QString getTableName() const override;
    std::unique_ptr<GoodReceivedNote> mapFromQuery(const QSqlQuery& query) const override;
    GoodReceivedNote* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const GoodReceivedNote& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif

