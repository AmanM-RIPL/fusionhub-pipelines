#ifndef PURCHASE_ORDER_H
#define PURCHASE_ORDER_H

#include <QString>
#include <QMetaType>
#include <QObject>

class PurchaseOrder: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)

    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)
    Q_PROPERTY(QString purchaseOrderList READ getPurchaseOrderList WRITE setPurchaseOrderList NOTIFY purchaseOrderListChanged)
    Q_PROPERTY(int amount READ getAmount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(QString materialName READ getMaterialName WRITE setMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(int quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int taxAmount READ getTaxAmount WRITE setTaxAmount NOTIFY taxAmountChanged)
    Q_PROPERTY(int taxWithHolding READ getTaxWithHolding WRITE setTaxWithHolding NOTIFY taxWithHoldingChanged)

    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)

public:
    explicit PurchaseOrder(QObject* parent = nullptr): QObject(parent) {}
    PurchaseOrder(int id, const QString& globalId, bool approvalStatus,
                    int vendorId, QString& vendorName, QString& purchaseOrderList, int amount , QString& materialName, int quantity, int taxAmount, int taxWithHolding,  int materialId, int unitOfMeasurementId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getVendorId() const { return vendorId; }
    QString getVendorName() const { return vendorName; }
    QString getPurchaseOrderList() const { return purchaseOrderList; }
    int getAmount() const { return amount; }
    QString getMaterialName() const { return materialName; }
    int getQuantity() const { return quantity; }
    int getTaxAmount() const { return taxAmount; }
    int getTaxWithHolding() const { return taxWithHolding; }


    int getMaterialId() const { return materialId; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }
    void setVendorName(const QString&  vendorName) { this->vendorName = vendorName; }
    void setPurchaseOrderList(const QString&  purchaseOrderList) { this->purchaseOrderList = purchaseOrderList; }
    void setAmount(int amount) { this->amount = amount; }
    void setMaterialName(const QString&  materialName) { this->materialName = materialName; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setTaxAmount(int taxAmount) { this->taxAmount = taxAmount; }
    void setTaxWithHolding(int taxWithHolding) { this->taxWithHolding = taxWithHolding; }

    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }



signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void vendorIdChanged();
    void purchaseOrderListChanged();
    void vendorNameChanged();
    void amountChanged();
    void materialNameChanged();
    void quantityChanged();
    void taxAmountChanged();
    void taxWithHoldingChanged();
    void materialIdChanged();
    void unitOfMeasurementIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString purchaseOrderName;
    QString purchaseOrderList;
    int vendorId = 0;
    QString vendorName ;
    int amount = 0;
    QString materialName ;
    int quantity = 0;
    int taxAmount = 0;
    int taxWithHolding = 0;
    int materialId = 0;
    int unitOfMeasurementId = 0;
};

Q_DECLARE_METATYPE(PurchaseOrder)

#endif //  PURCHASE_ORDER_H
