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
