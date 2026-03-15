#ifndef VENDOR_REPOSITORY_H
#define VENDOR_REPOSITORY_H
#include "repositories/abstract_repository.h"
#include "models/vendor.h"
#include <memory>
#include <QObject>

class VendorRepository : public QObject, public AbstractRepository<Vendor>
{
    Q_OBJECT
public:
    explicit VendorRepository(QObject* parent = nullptr);
    Q_INVOKABLE std::unique_ptr<Vendor> findById(int id) override;
    std::vector<std::unique_ptr<Vendor>> findAll() override;
    Q_INVOKABLE std::vector<Vendor*> findAllQML();
    Q_INVOKABLE std::vector<Vendor*> findDashboardTableQML(int vendorId);
    Q_INVOKABLE std::vector<Vendor*> findDashboardQML(int vendorId);


    bool save(const Vendor& entity) override;
    Q_INVOKABLE bool saveQML(Vendor* entity);
    bool update(const Vendor& entity) override;
    bool deleteById(int id) override;
    std::vector<std::unique_ptr<Vendor>> findByApprovalStatus(bool status) override;
protected:
    QString getTableName() const override;
    std::unique_ptr<Vendor> mapFromQuery(const QSqlQuery& query) const override;
    Vendor* mapFromQueryQML(const QSqlQuery& query, QObject* parent) const;
    void bindEntityToQuery(QSqlQuery& query, const Vendor& entity) const override;
    QString getInsertQuery() const override;
    QString getUpdateQuery() const override;
};
#endif
