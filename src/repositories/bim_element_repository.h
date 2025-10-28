#ifndef BIM_ELEMENT_REPOSITORY_H
#define BIM_ELEMENT_REPOSITORY_H

#include <QObject>

#include "repositories/abstract_repository.h"
#include "models/bim_element.h"

class BIMElementRepository : public QObject, public AbstractRepository<BIMElement>
{
    Q_OBJECT
public:
    explicit BIMElementRepository(QObject *parent = nullptr);
    Q_INVOKABLE std::unique_ptr<BIMElement> findById(int id) override;
    std::vector<std::unique_ptr<BIMElement>> findAll() override;
    Q_INVOKABLE std::vector<BIMElement*> findAllQML();
    bool save(const BIMElement& entity) override;
    Q_INVOKABLE bool saveQML(BIMElement* entity);
    bool update(const BIMElement& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<BIMElement>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<BIMElement> mapFromQuery(const QSqlQuery& query) const override;
    BIMElement* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const BIMElement& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};

#endif // BIM_ELEMENT_REPOSITORY_H
