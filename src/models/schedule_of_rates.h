#ifndef SCHEDULE_OF_RATES_H
#define SCHEDULE_OF_RATES_H

#include <QString>
#include <QMetaType>
#include <QObject>

class ScheduleOfRates: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString scheduleOfRatesName READ getScheduleOfRatesName WRITE setScheduleOfRatesName NOTIFY scheduleOfRatesNameChanged)

public:
    explicit ScheduleOfRates(QObject* parent = nullptr): QObject(parent) {}
    ScheduleOfRates(int id, const QString& globalId, bool approvalStatus,
             const QString& scheduleOfRatesName, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getScheduleOfRatesName() const { return scheduleOfRatesName; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setScheduleOfRatesName(const QString& scheduleOfRatesName) { this->scheduleOfRatesName = scheduleOfRatesName; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void scheduleOfRatesNameChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString scheduleOfRatesName;
};

Q_DECLARE_METATYPE(ScheduleOfRates)

#endif //  SCHEDULE_OF_RATES_H
