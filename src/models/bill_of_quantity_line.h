#ifndef BILLOFQUANTITYLINE_H
#define BILLOFQUANTITYLINE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class BillOfQuantityLine: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString billOfQuantityLineName READ getBillOfQuantityLineName WRITE setBillOfQuantityLineName NOTIFY billOfQuantityLineNameChanged)
    Q_PROPERTY(int billOfQuantityId READ getBillOfQuantityId WRITE setBillOfQuantityId NOTIFY billOfQuantityIdChanged)
    Q_PROPERTY(int taskId READ getTaskId WRITE setTaskId NOTIFY taskIdChanged)


public:
    explicit BillOfQuantityLine(QObject* parent = nullptr): QObject(parent) {}
    BillOfQuantityLine(int id,
             const QString& globalId, bool approvalStatus,
             const QString& billOfQuantityLineName,
             int billOfQuantityId,
             int taskId,
            QObject* parent = nullptr
        );

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getBillOfQuantityLineName() const { return billOfQuantityLineName; }
    int getBillOfQuantityId() const { return billOfQuantityId; }
    int getTaskId() const { return taskId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setBillOfQuantityLineName(const QString& billOfQuantityLineName) { this->billOfQuantityLineName = billOfQuantityLineName; }
    void setBillOfQuantityId(int billOfQuantityId) { this->billOfQuantityId = billOfQuantityId; }
    void setTaskId(int taskId) { this->taskId = taskId; }


signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void billOfQuantityLineNameChanged();
    void billOfQuantityIdChanged();
    void taskIdChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString billOfQuantityLineName;
    int billOfQuantityId = 0;
    int taskId = 0;
};

Q_DECLARE_METATYPE(BillOfQuantityLine)

#endif // BILLOFQUANTITYLINE_H
