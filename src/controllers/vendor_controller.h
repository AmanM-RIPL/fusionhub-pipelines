#ifndef VENDOR_CONTROLLER_H
#define VENDOR_CONTROLLER_H

#include <QObject>
#include "repositories/vendor_repository.h"
#include "repositories/draft_entity_repository.h"

class VendorController: public QObject
{
    Q_OBJECT
public:
    //VendorController();
    explicit VendorController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &vendorName, const QString &vendorAddress, const QString &vendorContactPerson,
                            const QString &vendorMobile, const QString &vendorEmail) const;
    Q_INVOKABLE std::vector<Vendor*> getVendorList(bool isApproved = false) const;


private:
    VendorRepository* m_vendorRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // VENDOR_CONTROLLER_H
