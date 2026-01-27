#ifndef SCHEDULE_OF_RATES_LINE_H
#define SCHEDULE_OF_RATES_LINE_H

#include <QString>
#include <QMetaType>
#include <QObject>
#include <QDebug>

class ScheduleOfRatesLine: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)

    Q_PROPERTY(int scheduleSetupId READ getScheduleSetupId WRITE setScheduleSetupId NOTIFY scheduleSetupIdChanged)
    Q_PROPERTY(int scheduleOfRatesId READ getScheduleOfRatesId WRITE setScheduleOfRatesId NOTIFY scheduleOfRatesIdChanged)
    Q_PROPERTY(QString costParam READ getCostParam WRITE setCostParam NOTIFY costParamChanged)
    Q_PROPERTY(QString resourceParam READ getResourceParam WRITE setResourceParam NOTIFY resourceParamChanged)

public:
    //ScheduleOfRatesLine() = default;
    explicit ScheduleOfRatesLine(QObject* parent = nullptr): QObject(parent) {}
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getScheduleSetupId()const {return scheduleSetupId;}
    int getScheduleOfRatesId()const {return scheduleOfRatesId;}
    QString getCostParam() const { return costParam; }
    QString getResourceParam() const { return resourceParam; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setScheduleSetupId(int scheduleSetupId){ this->scheduleSetupId = scheduleSetupId; }
    void setScheduleOfRatesId(int scheduleOfRatesId){ this->scheduleOfRatesId = scheduleOfRatesId; }
    void setCostParam(const QString& costParam) {
        if(!costParam.isEmpty())
        {
            this->costParam = costParam;
        }
    }
    void setResourceParam(const QString& resourceParam) { this->resourceParam = resourceParam; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void scheduleSetupIdChanged();
    void scheduleOfRatesIdChanged();
    void costParamChanged();
    void resourceParamChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int scheduleSetupId;
    int scheduleOfRatesId;
    QString costParam;
    QString resourceParam;

};
Q_DECLARE_METATYPE(ScheduleOfRatesLine)
#endif
