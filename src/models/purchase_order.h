#ifndef PURCHASE_ORDER_H
#define PURCHASE_ORDER_H
#include <QString>
#include <QMetaType>
class PurchaseOrder {
public:
    PurchaseOrder() = default;
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
};
Q_DECLARE_METATYPE(PurchaseOrder)
#endif
