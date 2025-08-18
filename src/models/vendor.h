#ifndef VENDOR_H
#define VENDOR_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Vendor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)
    Q_PROPERTY(QString vendorAddress READ getVendorAddress WRITE setVendorAddress NOTIFY vendorAddressChanged)
    Q_PROPERTY(QString vendorContactPerson READ getVendorContactPerson WRITE setVendorContactPerson NOTIFY vendorContactPersonChanged)
    Q_PROPERTY(QString vendorMobile READ getVendorMobile WRITE setVendorMobile NOTIFY vendorMobileChanged)
    Q_PROPERTY(QString vendorEmail READ getVendorEmail WRITE setVendorEmail NOTIFY vendoEmailChanged)

public:
    explicit Vendor(QObject* parent = nullptr): QObject(parent) {}
    Vendor(int id, const QString& globalId, bool approvalStatus,
           const QString& vendorName, const QString& vendorAddress,
           const QString& vendorContactPerson, const QString& vendorMobile,
           const QString& vendorEmail, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getVendorName() const { return vendorName; }
    QString getVendorAddress() const { return vendorAddress; }
    QString getVendorContactPerson() const { return vendorContactPerson; }
    QString getVendorMobile() const { return vendorMobile; }
    QString getVendorEmail() const { return vendorEmail; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setVendorName(const QString& vendorName) { this->vendorName = vendorName; }
    void setVendorAddress(const QString& vendorAddress) { this->vendorAddress = vendorAddress; }
    void setVendorContactPerson(const QString& vendorContactPerson) { this->vendorContactPerson = vendorContactPerson; }
    void setVendorMobile(const QString& vendorMobile) { this->vendorMobile = vendorMobile; }
    void setVendorEmail(const QString& vendorEmail) { this->vendorEmail = vendorEmail; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void vendorNameChanged();
    void vendorAddressChanged();
    void vendorContactPersonChanged();
    void vendorMobileChanged();
    void vendoEmailChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString vendorName;
    QString vendorAddress;
    QString vendorContactPerson;
    QString vendorMobile;
    QString vendorEmail;
};

Q_DECLARE_METATYPE(Vendor)

#endif // VENDOR_H
