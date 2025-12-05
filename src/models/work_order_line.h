#ifndef WORK_ORDER_LINE_H
#define WORK_ORDER_LINE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class WorkOrderLine: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int workOrderId READ getWorkOrderId WRITE setWorkOrderId NOTIFY workOrderIdChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int taskId READ getTaskId WRITE setTaskId NOTIFY taskIdChanged)
    Q_PROPERTY(double amount READ getAmount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(double taxAmount READ getTaxAmount WRITE setTaxAmount NOTIFY taxAmountChanged)
    Q_PROPERTY(double taxWithHolding READ getTaxWithHolding WRITE setTaxWithHolding NOTIFY taxWithHoldingChanged)
    Q_PROPERTY(double retentionAmount READ getRetentionAmount WRITE setRetentionAmount NOTIFY retentionAmountChanged)

    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)
    Q_PROPERTY(QString taskName READ getTaskName WRITE setTaskName NOTIFY taskNameChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)


public:
    explicit WorkOrderLine(QObject* parent = nullptr): QObject(parent) {}
    WorkOrderLine(int id, const QString& globalId, bool approvalStatus,int workOrderId, const QString& description, int taskId,
                     double amount , double taxAmount, double taxWithHolding, double retentionAmount,  QObject* parent = nullptr);

    // --- Getters ---
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getWorkOrderId() const { return workOrderId; }
    int getTaskId() const { return taskId; }
    QString getDescription() const { return description; }
    double getAmount() const { return amount; }
    double getTaxAmount() const { return taxAmount; }
    double getTaxWithHolding() const { return taxWithHolding; }
    double getRetentionAmount() const { return retentionAmount; }
    int getVendorId() const { return vendorId; }
    QString getTaskName() const { return taskName; }
    QString getVendorName() const { return vendorName; }


    // --- Setters ---
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setWorkOrderId(int workOrderId) { this->workOrderId = workOrderId; }
    void setDescription(const QString& description) { this->description = description; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setAmount(double amount) { this->amount = amount; }
    void setTaxAmount(double taxAmount) { this->taxAmount = taxAmount; }
    void setTaxWithHolding(double taxWithHolding) { this->taxWithHolding = taxWithHolding; }
    void setRetentionAmount(double retentionAmount) { this->retentionAmount = retentionAmount; }
    void setVendorId(int vendorId){this->vendorId = vendorId;}
    void setTaskName(const QString& taskName){this->taskName = taskName;}
    void setVendorName(const QString& vendorName){this->vendorName = vendorName;}


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void workOrderIdChanged();
    void descriptionChanged();
    void taskIdChanged();
    void amountChanged();
    void taxAmountChanged();
    void taxWithHoldingChanged();
    void retentionAmountChanged();
    void vendorIdChanged();
    void taskNameChanged();
    void vendorNameChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int workOrderId = 0;
    QString description;
    int taskId = 0;
    double amount = 0;
    double taxAmount = 0;
    double taxWithHolding = 0;
    double retentionAmount = 0;
    int vendorId = 0;
    QString taskName;
    QString vendorName;
};

Q_DECLARE_METATYPE(WorkOrderLine)

#endif //  PURCHASE_ORDER_LINE_H

