#ifndef WORK_ORDER_H
#define WORK_ORDER_H

#include <QString>
#include <QMetaType>
#include <QObject>

class WorkOrder: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString workOrderName READ getWorkOrderName WRITE setWorkOrderName NOTIFY workOrderNameChanged)
    Q_PROPERTY(QString vendorName READ getVendorName WRITE setVendorName NOTIFY vendorNameChanged)

    Q_PROPERTY(int vendorId READ getVendorId WRITE setVendorId NOTIFY vendorIdChanged)
    Q_PROPERTY(QString workOrderLineData READ getWorkOrderLineData WRITE setWorkOrderLineData NOTIFY workOrderLineDataChanged)


public:
    explicit WorkOrder(QObject* parent = nullptr): QObject(parent) {}
    WorkOrder(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
                    const QString& workOrderName,const QString& vendorName, int vendorId , QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    QString getWorkOrderName() const { return workOrderName; }
    QString getVendorName() const { return vendorName; }
    int getVendorId() const { return vendorId; }
    QString getWorkOrderLineData() const { return workOrderLineData; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setWorkOrderName(const QString& workOrderName) { this->workOrderName = workOrderName; }
    void setVendorName(const QString& vendorName) { this->vendorName = vendorName; }
    void setVendorId(int vendorId) { this->vendorId = vendorId; }
    void setWorkOrderLineData(const QString& workOrderLineData) { this->workOrderLineData = workOrderLineData; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void workOrderNameChanged();
    void vendorNameChanged();
    void vendorIdChanged();
    void workOrderLineDataChanged();

private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    QString workOrderName;
    QString vendorName;
    int vendorId = 0;
    QString workOrderLineData;
};

Q_DECLARE_METATYPE(WorkOrder)

#endif //  WORK_ORDER_H
