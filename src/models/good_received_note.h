#ifndef GOOD_RECEIVED_NOTE_H
#define GOOD_RECEIVED_NOTE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class GoodReceivedNote: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int purchaseOrderLineId READ getPurchaseOrderLineId WRITE setPurchaseOrderLineId NOTIFY purchaseOrderLineIdChanged)

public:
    explicit GoodReceivedNote(QObject* parent = nullptr): QObject(parent) {}
    GoodReceivedNote(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
             int quantity, int purchaseOrderLineId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    int getQuantity() const { return quantity; }
    int getPurchaseOrderLineId() const { return purchaseOrderLineId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setPurchaseOrderLineId(int purchaseOrderLineId) { this->purchaseOrderLineId = purchaseOrderLineId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void quantityChanged();
    void purchaseOrderLineIdChanged();


private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    int quantity = 0 ;
    int purchaseOrderLineId = 0;
};

Q_DECLARE_METATYPE(GoodReceivedNote)

#endif

