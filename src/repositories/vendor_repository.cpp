#include "repositories/vendor_repository.h"
VendorRepository::VendorRepository(QObject* parent) : QObject(parent)
{

}
std::unique_ptr<Vendor> VendorRepository::findById(int id)
{
    return nullptr;
}
std::vector<std::unique_ptr<Vendor>> VendorRepository::findAll()
{
    return {};
}

std::vector<Vendor*> VendorRepository::findAllQML() {
    std::vector<Vendor*> vendors;

    QSqlQuery query(dbManager->getDatabase());

    if (query.exec("SELECT * FROM Vendor")) {
        while (query.next()) {
            vendors.push_back(mapFromQueryQML(query, this));
        }
    }
    return vendors;
}

std::vector<Vendor*> VendorRepository::findDashboardQML(int vendorId)
{
    std::vector<Vendor*> vendors;

    QSqlQuery query;

    query.prepare(R"(
        SELECT
            COALESCE(SUM(wol.dollar_value),0) AS work_order,
            COALESCE(SUM(wbl.dollar_value),0) AS billing,
            COALESCE(SUM(wol.dollar_value),0) - COALESCE(SUM(wbl.dollar_value),0) AS remaining

        FROM WorkOrder wo

        LEFT JOIN WorkOrderLine wol
            ON wol.work_order_id = wo.id

         LEFT JOIN WorkBillingLine wbl
         ON wbl.work_order_line_id = wol.id

        WHERE wo.vendor_id = :vendorId
    )");

    query.bindValue(":vendorId", vendorId);

    if(query.exec() && query.next()) {

        Vendor* v = new Vendor();

        v->setTotalWorkOrderAmount(query.value("work_order").toDouble());
        v->setTotalBillingAmount(query.value("billing").toDouble());
        v->setTotalRemaining(query.value("remaining").toDouble());

        vendors.push_back(v);
    }

    return vendors;

}

std::vector<Vendor*> VendorRepository::findDashboardTableQML(int vendorId) {
    std::vector<Vendor*> workOrders;
    QSqlQuery query(dbManager->getDatabase());

    QString sql = R"(
        SELECT
            wo.id,
            wo.description AS workOrderDescription,
            COALESCE(SUM(wol.dollar_value), 0) AS totalAmount,
            COUNT(wol.id) AS totalBilling
        FROM WorkOrder wo

        LEFT JOIN WorkOrderLine wol ON wol.work_order_id = wo.id
        WHERE wo.vendor_id = :vendorId
        GROUP BY wo.id, wo.description
    )";

    if (!query.prepare(sql)) {
        qDebug() << "SQL Prepare Failed:" << query.lastError().text();
        return workOrders;
    }

    query.bindValue(":vendorId", vendorId);

    if (query.exec()) {
        while (query.next()) {
            Vendor* wo = new Vendor();

            wo->setVendorName(query.value("workOrderDescription").toString());
            wo->setTotalAmount(query.value("totalAmount").toDouble());
            wo->setTotalBilling(query.value("totalBilling").toInt());

            workOrders.push_back(wo);
        }

        if (workOrders.empty()) {
            qDebug() << "Query executed but returned 0 rows for vendorId:" << vendorId;
        }
    } else {
        qDebug() << "Query Exec Error:" << query.lastError().text();
    }

    return workOrders;
}

bool VendorRepository::save(const Vendor& entity) { return false; }

bool VendorRepository::saveQML(Vendor* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool VendorRepository::update(const Vendor& entity) { return false; }
bool VendorRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<Vendor>> VendorRepository::findByApprovalStatus(bool status) { return {}; }
QString VendorRepository::getTableName() const { return "Vendor"; }
std::unique_ptr<Vendor> VendorRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }
Vendor* VendorRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto vendor = new Vendor(parent);
    vendor->setId(query.value("id").toInt());
    vendor->setGlobalId(query.value("global_id").toString());
    vendor->setApprovalStatus(query.value("approval_status").toString());
    vendor->setVendorName(query.value("vendor_name").toString());
    vendor->setVendorAddress(query.value("vendor_address").toString());
    vendor->setVendorContactPerson(query.value("vendor_contact_person").toString());
    vendor->setVendorEmail(query.value("vendor_email").toString());
    vendor->setVendorMobile(query.value("vendor_mobile").toString());

    return vendor;
}

void VendorRepository::bindEntityToQuery(QSqlQuery& query, const Vendor& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getVendorName());
    query.addBindValue(entity.getVendorAddress());
    query.addBindValue(entity.getVendorContactPerson());
    query.addBindValue(entity.getVendorEmail());
    query.addBindValue(entity.getVendorMobile());
}
QString VendorRepository::getInsertQuery() const {
    return "INSERT INTO Vendor (global_id, approval_status, vendor_name, vendor_address, vendor_contact_person, vendor_email, vendor_mobile) VALUES (?, ?, ?, ?, ?, ?, ?)";
}
QString VendorRepository::getUpdateQuery() const {
    return "UPDATE User SET global_id = ?, approval_status = ?, vendor_name = ?, vendor_address = ?, vendor_contact_person = ?, vendor_email = ?, vendor_mobile = ? WHERE id = ?";
}
