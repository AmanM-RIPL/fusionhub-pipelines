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
    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(int quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)
    Q_PROPERTY(double amount READ getAmount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(double taxAmount READ getTaxAmount WRITE setTaxAmount NOTIFY taxAmountChanged)
    Q_PROPERTY(double taxWithHolding READ getTaxWithHolding WRITE setTaxWithHolding NOTIFY taxWithHoldingChanged)

public:
    explicit PurchaseOrderLine(QObject* parent = nullptr): QObject(parent) {}
    PurchaseOrderLine(int id, const QString& globalId, bool approvalStatus,int purchaseOrderId, int materialId,
                      int quantity, int unitOfMeasurementId, double amount , double taxAmount, double taxWithHolding, QObject* parent = nullptr);

    // --- Getters ---
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getPurchaseOrderId() const { return purchaseOrderId; }
    int getMaterialId() const { return materialId; }
    double getAmount() const { return amount; }
    int getQuantity() const { return quantity; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }
    double getTaxAmount() const { return taxAmount; }
    double getTaxWithHolding() const { return taxWithHolding; }

    // --- Setters ---
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setPurchaseOrderId(int purchaseOrderId) { this->purchaseOrderId = purchaseOrderId; }
    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setAmount(double amount) { this->amount = amount; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }
    void setTaxAmount(double taxAmount) { this->taxAmount = taxAmount; }
    void setTaxWithHolding(double taxWithHolding) { this->taxWithHolding = taxWithHolding; }


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void purchaseOrderIdChanged();
    void materialIdChanged();
    void amountChanged();
    void unitOfMeasurementIdChanged();
    void quantityChanged();
    void taxAmountChanged();
    void taxWithHoldingChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int purchaseOrderId = 0;
    int materialId = 0;
    double amount = 0;
    int unitOfMeasurementId = 0;
    int quantity = 0;
    double taxAmount = 0;
    double taxWithHolding = 0;
};

Q_DECLARE_METATYPE(PurchaseOrderLine)

#endif //  PURCHASE_ORDER_LINE_H
