#ifndef WORK_BILLING_H
#define WORK_BILLING_H

#include <QString>
#include <QMetaType>
#include <QObject>

class WorkBilling: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString workBillingName READ getWorkBillingName WRITE setWorkBillingName NOTIFY workBillingNameChanged)
    Q_PROPERTY(QString workOrderName READ getWorkOrderName WRITE setWorkOrderName NOTIFY workOrderNameChanged)
    Q_PROPERTY(int workOrderId READ getWorkOrderId WRITE setWorkOrderId NOTIFY workOrderIdChanged)

public:
    explicit WorkBilling(QObject* parent = nullptr): QObject(parent) {}
    WorkBilling(int id, const QString& globalId, bool approvalStatus,
              const QString& workBillingName,const QString& workOrderName, int workOrderId , QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getWorkBillingName() const { return workBillingName; }
    QString getWorkOrderName() const { return workOrderName; }
    int getWorkOrderId() const { return workOrderId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setWorkBillingName(const QString& workBillingName) { this->workBillingName = workBillingName; }
    void setWorkOrderName(const QString& workOrderName) { this->workOrderName = workOrderName; }
    void setWorkOrderId(int workOrderId) { this->workOrderId = workOrderId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void workBillingNameChanged();
    void workOrderNameChanged();
    void workOrderIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString workBillingName;
    QString workOrderName;
    int workOrderId = 0;
};

Q_DECLARE_METATYPE(WorkBilling)

#endif //  WORK_BILLING_H
