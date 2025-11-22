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
    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)

public:
    explicit PurchaseOrder(QObject* parent = nullptr): QObject(parent) {}
    PurchaseOrder(int id, const QString& globalId, bool approvalStatus,
                    int vendorId, int materialId, int unitOfMeasurementId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getVendorId() const { return vendorId; }
    int getMaterialId() const { return materialId; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }
    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }



signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void vendorIdChanged();
    void materialIdChanged();
    void unitOfMeasurementIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString purchaseOrderName;
    int vendorId = 0;
    int materialId = 0;
    int unitOfMeasurementId = 0;
};

Q_DECLARE_METATYPE(PurchaseOrder)

#endif //  PURCHASE_ORDER_H
