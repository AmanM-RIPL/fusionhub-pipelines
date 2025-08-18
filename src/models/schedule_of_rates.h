#ifndef SCHEDULE_OF_RATES_H
#define SCHEDULE_OF_RATES_H
#include <QString>
#include <QMetaType>
class ScheduleOfRates {
public:
    ScheduleOfRates() = default;
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
};
Q_DECLARE_METATYPE(ScheduleOfRates)
#endif
