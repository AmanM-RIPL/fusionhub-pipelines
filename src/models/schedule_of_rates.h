#ifndef SCHEDULE_OF_RATES_H
#define SCHEDULE_OF_RATES_H

#include <QString>
#include <QMetaType>
#include <QObject>

class ScheduleOfRates: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString scheduleOfRatesName READ getScheduleOfRatesName WRITE setScheduleOfRatesName NOTIFY scheduleOfRatesNameChanged)

public:
    explicit ScheduleOfRates(QObject* parent = nullptr): QObject(parent) {}
    ScheduleOfRates(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
             const QString& scheduleOfRatesName, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    QString getScheduleOfRatesName() const { return scheduleOfRatesName; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setScheduleOfRatesName(const QString& scheduleOfRatesName) { this->scheduleOfRatesName = scheduleOfRatesName; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void scheduleOfRatesNameChanged();


private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    QString scheduleOfRatesName;
};

Q_DECLARE_METATYPE(ScheduleOfRates)

#endif //  SCHEDULE_OF_RATES_H
