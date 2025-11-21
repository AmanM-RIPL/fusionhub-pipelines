#ifndef BILL_OF_QUANTITY_H
#define BILL_OF_QUANTITY_H

#include <QString>
#include <QMetaType>
#include <QObject>

class BillOfQuantity: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int scheduleId READ getScheduleId WRITE setScheduleId NOTIFY scheduleIdChanged)

public:
    explicit BillOfQuantity(QObject* parent = nullptr): QObject(parent) {}
    BillOfQuantity(int id, const QString& globalId, bool approvalStatus,
             const QString& description,
             int scheduleId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getDescription() const { return description; }
    int getScheduleId() const { return scheduleId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setDescription(const QString& description) { this->description = description; }
    void setScheduleId(int scheduleId) { this->scheduleId = scheduleId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void descriptionChanged();
    void scheduleIdChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString description;
    int scheduleId = 0;
};

Q_DECLARE_METATYPE(BillOfQuantity)

#endif // BILL_OF_QUANTITY_H
