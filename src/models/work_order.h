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
    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)


public:
    explicit WorkOrder(QObject* parent = nullptr): QObject(parent) {}
    WorkOrder(int id, const QString& globalId, bool approvalStatus,
                    const QString& workOrderName, int vendorId , QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getWorkOrderName() const { return workOrderName; }
    int getVendorId() const { return vendorId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setWorkOrderName(const QString& workOrderName) { this->workOrderName = workOrderName; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void workOrderNameChanged();
    void vendorIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString workOrderName;
    int vendorId = 0;
};

Q_DECLARE_METATYPE(WorkOrder)

#endif //  WORK_ORDER_H
