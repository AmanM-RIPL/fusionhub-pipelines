#ifndef WORK_ORDER_H
#define WORK_ORDER_H

#include <QString>
#include <QMetaType>
#include <QObject>

class WorkOrder: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString workOrderName READ getWorkOrderName WRITE setWorkOrderName NOTIFY workOrderNameChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)

    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)

     Q_PROPERTY(QString workOrderLineData READ getWorkOrderLineData WRITE setWorkOrderLineData NOTIFY workOrderLineDataChanged)


public:
    explicit WorkOrder(QObject* parent = nullptr): QObject(parent) {}
    WorkOrder(int id, const QString& globalId, bool approvalStatus,
                    const QString& workOrderName,const QString& vendorName, int vendorId , QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getWorkOrderName() const { return workOrderName; }
    QString getVendorName() const { return vendorName; }
    int getVendorId() const { return vendorId; }
    QString getWorkOrderLineData() const { return workOrderLineData; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setWorkOrderName(const QString& workOrderName) { this->workOrderName = workOrderName; }
    void setVendorName(const QString& vendorName) { this->vendorName = vendorName; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }
    void setWorkOrderLineData(const QString& workOrderLineData) { this->workOrderLineData = workOrderLineData; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void workOrderNameChanged();
    void vendorNameChanged();
    void vendorIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString workOrderName;
    QString vendorName;
    int vendorId = 0;
};

Q_DECLARE_METATYPE(WorkOrder)

#endif //  WORK_ORDER_H
