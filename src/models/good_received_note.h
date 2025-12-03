#ifndef GOOD_RECEIVED_NOTE_H
#define GOOD_RECEIVED_NOTE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class GoodReceivedNote: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int purchaseOrderLineId READ getPurchaseOrderLineId WRITE setPurchaseOrderLineId NOTIFY purchaseOrderLineIdChanged)

public:
    explicit GoodReceivedNote(QObject* parent = nullptr): QObject(parent) {}
    GoodReceivedNote(int id, const QString& globalId, bool approvalStatus,
             int quantity, int purchaseOrderLineId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getQuantity() const { return quantity; }
    int getPurchaseOrderLineId() const { return purchaseOrderLineId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setPurchaseOrderLineId(int purchaseOrderLineId) { this->purchaseOrderLineId = purchaseOrderLineId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void quantityChanged();
    void purchaseOrderLineIdChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int quantity = 0 ;
    int purchaseOrderLineId = 0;
};

Q_DECLARE_METATYPE(GoodReceivedNote)

#endif

