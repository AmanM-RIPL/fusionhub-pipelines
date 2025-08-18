#ifndef SCHEDULE_SETUP_H
#define SCHEDULE_SETUP_H
#include <QString>
#include <QMetaType>
#include <QObject>

class ScheduleSetup: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString scheduleName READ getScheduleName WRITE setScheduleName NOTIFY scheduleNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString costParameter READ getCostParameter WRITE setCostParameter NOTIFY costParameterChanged)
    Q_PROPERTY(QString resourceParameter READ getResourceParameter WRITE setResourceParameter NOTIFY resourceParameterChanged)

public:
    explicit ScheduleSetup(QObject* parent = nullptr): QObject(parent) {}
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getScheduleName() const { return scheduleName; }
    QString getDescription() const { return description; }
    QString getCostParameter() const { return costParameter; }
    QString getResourceParameter() const { return resourceParameter; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setScheduleName(const QString& scheduleName) { this->scheduleName = scheduleName; }
    void setDescription(const QString& description) { this->description = description; }
    void setCostParameter(const QString& costParameter) { this->costParameter = costParameter; }
    void setResourceParameter(const QString& resourceParameter) { this->resourceParameter = resourceParameter; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void scheduleNameChanged();
    void descriptionChanged();
    void costParameterChanged();
    void resourceParameterChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString scheduleName;
    QString description;
    QString costParameter;
    QString resourceParameter;
};

Q_DECLARE_METATYPE(ScheduleSetup)
#endif
