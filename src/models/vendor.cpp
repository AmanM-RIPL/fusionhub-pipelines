#include "models/vendor.h"

Vendor::Vendor(int id, const QString& globalId, const QString& approvalStatus,
               const QString& vendorName, const QString& vendorAddress,
               const QString& vendorContactPerson, const QString& vendorMobile,
               const QString& vendorEmail, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
      vendorName(vendorName), vendorAddress(vendorAddress),
      vendorContactPerson(vendorContactPerson), vendorMobile(vendorMobile),
      vendorEmail(vendorEmail)
{
}
