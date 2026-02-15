#ifndef VENDOR_H
#define VENDOR_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Vendor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)
    Q_PROPERTY(QString vendorAddress READ getVendorAddress WRITE setVendorAddress NOTIFY vendorAddressChanged)
    Q_PROPERTY(QString vendorContactPerson READ getVendorContactPerson WRITE setVendorContactPerson NOTIFY vendorContactPersonChanged)
    Q_PROPERTY(QString vendorMobile READ getVendorMobile WRITE setVendorMobile NOTIFY vendorMobileChanged)
    Q_PROPERTY(QString vendorEmail READ getVendorEmail WRITE setVendorEmail NOTIFY vendoEmailChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)


public:
    explicit Vendor(QObject* parent = nullptr): QObject(parent) {}
    Vendor(int id, const QString& globalId, const QString& approvalStatus,
           const QString& vendorName, const QString& vendorAddress,
           const QString& vendorContactPerson, const QString& vendorMobile,
           const QString& vendorEmail, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    QString getVendorName() const { return vendorName; }
    QString getVendorAddress() const { return vendorAddress; }
    QString getVendorContactPerson() const { return vendorContactPerson; }
    QString getVendorMobile() const { return vendorMobile; }
    QString getVendorEmail() const { return vendorEmail; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setVendorName(const QString& vendorName) { this->vendorName = vendorName; }
    void setVendorAddress(const QString& vendorAddress) { this->vendorAddress = vendorAddress; }
    void setVendorContactPerson(const QString& vendorContactPerson) { this->vendorContactPerson = vendorContactPerson; }
    void setVendorMobile(const QString& vendorMobile) { this->vendorMobile = vendorMobile; }
    void setVendorEmail(const QString& vendorEmail) { this->vendorEmail = vendorEmail; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void vendorNameChanged();
    void vendorAddressChanged();
    void vendorContactPersonChanged();
    void vendorMobileChanged();
    void vendoEmailChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();

private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    QString vendorName;
    QString vendorAddress;
    QString vendorContactPerson;
    QString vendorMobile;
    QString vendorEmail;
    int nextApprovingUser = 0;
    int createdByUser = 0;
};

Q_DECLARE_METATYPE(Vendor)

#endif // VENDOR_H
