#ifndef PURCHASE_ORDER_LINE_H
#define PURCHASE_ORDER_LINE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class PurchaseOrderLine: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int purchaseOrderId READ getPurchaseOrderId WRITE setPurchaseOrderId NOTIFY purchaseOrderIdChanged)

    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)

    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(QString materialName READ getMaterialName WRITE setMaterialName NOTIFY materialNameChanged)

    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)
    Q_PROPERTY(QString unitOfMeasurementName READ getUnitOfMeasurementName WRITE setUnitOfMeasurementName NOTIFY unitOfMeasurementNameChanged)

    Q_PROPERTY(int quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(double amount READ getAmount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(double taxAmount READ getTaxAmount WRITE setTaxAmount NOTIFY taxAmountChanged)
    Q_PROPERTY(double taxWithHolding READ getTaxWithHolding WRITE setTaxWithHolding NOTIFY taxWithHoldingChanged)

public:
    explicit PurchaseOrderLine(QObject* parent = nullptr): QObject(parent) {}
    PurchaseOrderLine(int id, const QString& globalId, bool approvalStatus,int purchaseOrderId, int materialId, const QString& materialName, int vendorId, const QString& vendorName,
                      int quantity, int unitOfMeasurementId, const QString& unitOfMeasurementName , double amount , double taxAmount, double taxWithHolding, QObject* parent = nullptr);

    // --- Getters ---
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getPurchaseOrderId() const { return purchaseOrderId; }
    int getVendorId() const { return vendorId; }
    QString getVendorName() const { return vendorName; }
    int getMaterialId() const { return materialId; }
    QString getMaterialName() const { return materialName; }
    double getAmount() const { return amount; }
    int getQuantity() const { return quantity; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }
    QString getUnitOfMeasurementName() const { return unitOfMeasurementName; }
    double getTaxAmount() const { return taxAmount; }
    double getTaxWithHolding() const { return taxWithHolding; }

    // --- Setters ---
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setPurchaseOrderId(int purchaseOrderId) { this->purchaseOrderId = purchaseOrderId; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }
    void setVendorName(const QString& vendorName) { this->vendorName = vendorName; }
    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setMaterialName(const QString& materialName) { this->materialName = materialName; }
    void setAmount(double amount) { this->amount = amount; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }
    void setUnitOfMeasurementName(const QString& unitOfMeasurementName) { this->unitOfMeasurementName = unitOfMeasurementName; }
    void setTaxAmount(double taxAmount) { this->taxAmount = taxAmount; }
    void setTaxWithHolding(double taxWithHolding) { this->taxWithHolding = taxWithHolding; }


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void purchaseOrderIdChanged();
    void vendorIdChanged();
    void vendorNameChanged();
    void materialIdChanged();
    void materialNameChanged();
    void amountChanged();
    void unitOfMeasurementIdChanged();
    void unitOfMeasurementNameChanged();
    void quantityChanged();
    void taxAmountChanged();
    void taxWithHoldingChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int purchaseOrderId = 0;
    int vendorId = 0;
    QString vendorName;
    int materialId = 0;
    QString materialName;
    double amount = 0;
    int unitOfMeasurementId = 0;
    QString unitOfMeasurementName;
    int quantity = 0;
    double taxAmount = 0;
    double taxWithHolding = 0;
};

Q_DECLARE_METATYPE(PurchaseOrderLine)

#endif //  PURCHASE_ORDER_LINE_H
