#ifndef BILL_OF_QUANTITY_H
#define BILL_OF_QUANTITY_H

#include <QString>
#include <QMetaType>
#include <QObject>

class BillOfQuantity: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int scheduleOfRatesId READ getScheduleOfRatesId WRITE setScheduleOfRatesId NOTIFY scheduleOfRatesIdChanged)

public:
    explicit BillOfQuantity(QObject* parent = nullptr): QObject(parent) {}
    BillOfQuantity(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
             const QString& description,
             int scheduleOfRatesId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    QString getDescription() const { return description; }
    int getScheduleOfRatesId() const { return scheduleOfRatesId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setDescription(const QString& description) { this->description = description; }
    void setScheduleOfRatesId(int scheduleOfRatesId) { this->scheduleOfRatesId = scheduleOfRatesId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void descriptionChanged();
    void scheduleOfRatesIdChanged();


private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    QString description;
    int scheduleOfRatesId = 0;
};

Q_DECLARE_METATYPE(BillOfQuantity)

#endif // BILL_OF_QUANTITY_H
