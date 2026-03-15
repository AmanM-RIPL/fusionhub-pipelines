#include "models/vendor.h"

Vendor::Vendor(int id, const QString& globalId, const QString& approvalStatus,
               const QString& vendorName, const QString& vendorAddress,
               const QString& vendorContactPerson, const QString& vendorMobile,
               const QString& vendorEmail,const double totalAmount, int totalBilling, const double totalWorkOrderAmount, const double totalBillingAmount, const double totalRemaining, QObject* parent)
    : QObject(parent), id(id), globalId(globalId), approvalStatus(approvalStatus),
      vendorName(vendorName), vendorAddress(vendorAddress),
      vendorContactPerson(vendorContactPerson), vendorMobile(vendorMobile),
    vendorEmail(vendorEmail),totalAmount(totalAmount),totalBilling(totalBilling),totalWorkOrderAmount(totalWorkOrderAmount),totalBillingAmount(totalBillingAmount),totalRemaining(totalRemaining)
{
}
