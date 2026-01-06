#ifndef WORK_BILLING_LINE_H
#define WORK_BILLING_LINE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class WorkBillingLine: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int workOrderId READ getWorkOrderId WRITE setWorkOrderId NOTIFY workOrderIdChanged)
    Q_PROPERTY(int workOrderLineId READ getWorkOrderLineId WRITE setWorkOrderLineId NOTIFY workOrderLineIdChanged)
    Q_PROPERTY(double amount READ getAmount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(double taxAmount READ getTaxAmount WRITE setTaxAmount NOTIFY taxAmountChanged)
    Q_PROPERTY(double taxWithHolding READ getTaxWithHolding WRITE setTaxWithHolding NOTIFY taxWithHoldingChanged)
    Q_PROPERTY(double retentionAmount READ getRetentionAmount WRITE setRetentionAmount NOTIFY retentionAmountChanged)
    Q_PROPERTY(QString workBillingName READ getWorkBillingName WRITE setWorkBillingName NOTIFY workBillingNameChanged)

    Q_PROPERTY(QString workOrderName READ getWorkOrderName WRITE setWorkOrderName NOTIFY workOrderNameChanged)
    Q_PROPERTY(QString workOrderLineName READ getWorkOrderLineName WRITE setWorkOrderLineName NOTIFY workOrderLineNameChanged)
public:
    // Default constructor
    explicit WorkBillingLine(QObject* parent = nullptr): QObject(parent) {}

      WorkBillingLine(int id, const QString& globalId, bool approvalStatus,int workOrderId, int workOrderLineId,const QString& workBillingName,
                    double amount , double taxAmount, double taxWithHolding, double retentionAmount,QObject* parent = nullptr);

    // Getters
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getWorkOrderId() const { return workOrderId; }
    int getWorkOrderLineId() const { return workOrderLineId; }
    QString getWorkBillingName() const { return workBillingName; }
    double getAmount() const { return amount; }
    double getTaxAmount() const { return taxAmount; }
    double getTaxWithHolding() const { return taxWithHolding; }
    double getRetentionAmount() const { return retentionAmount; }
    QString getWorkOrderName() const { return workOrderName; }
    QString getWorkOrderLineName() const { return workOrderLineName; }

    // Setters
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setWorkOrderId(int workOrderId) { this->workOrderId = workOrderId; }
    void setWorkOrderLineId(int workOrderLineId) { this->workOrderLineId = workOrderLineId; }
    void setWorkBillingName(const QString& workBillingName){this->workBillingName = workBillingName;}
    void setAmount(double amount) { this->amount = amount; }
    void setTaxAmount(double taxAmount) { this->taxAmount = taxAmount; }
    void setTaxWithHolding(double taxWithHolding) { this->taxWithHolding = taxWithHolding; }
    void setRetentionAmount(double retentionAmount) { this->retentionAmount = retentionAmount; }
    void setWorkOrderName(const QString& workOrderName){this->workOrderName = workOrderName;}
    void setWorkOrderLineName(const QString& workOrderLineName){this->workOrderLineName = workOrderLineName;}

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void workOrderIdChanged();
    void workOrderLineIdChanged();
    void workBillingNameChanged();
    void amountChanged();
    void taxAmountChanged();
    void taxWithHoldingChanged();
    void retentionAmountChanged();
    void workOrderNameChanged();
    void workOrderLineNameChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int workOrderId = 0;
    int workOrderLineId = 0;
    QString workBillingName;
    double amount = 0;
    double taxAmount = 0;
    double taxWithHolding = 0;
    double retentionAmount = 0;
    QString workOrderName;
    QString workOrderLineName;
};

Q_DECLARE_METATYPE(WorkBillingLine)

#endif // WORK_BILLING_LINE_H
