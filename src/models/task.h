#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Task: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString taskName READ getTaskName WRITE setTaskName NOTIFY taskNameChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString bimElement READ getBimElement WRITE setBimElement NOTIFY bimElementChanged)
    Q_PROPERTY(QString startDate READ getStartDate WRITE setStartDate NOTIFY startDateChanged)
    Q_PROPERTY(QString endDate READ getEndDate WRITE setEndDate NOTIFY endDateChanged)


public:
    explicit Task(QObject* parent = nullptr): QObject(parent) {}
    Task(int id, const QString& globalId, bool approvalStatus,
             const QString& taskName,
             const QString& description,
             const QString& bimElement,
             const QString& startDate,
             const QString& endDate,
            QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getTaskName() const { return taskName; }
    QString getDescription() const { return description; }
    QString getBimElement() const { return bimElement; }
    QString getStartDate() const { return startDate; }
    QString getEndDate() const { return endDate; }


    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setTaskName(const QString& taskName) { this->taskName = taskName; }
    void setDescription(const QString& description) { this->description = description; }
    void setBimElement(const QString& bimElement) { this->bimElement = bimElement; }
    void setStartDate(const QString& startDate) { this->startDate = startDate; }
    void setEndDate(const QString& endDate) { this->endDate = endDate; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void taskNameChanged();
    void descriptionChanged();
    void bimElementChanged();
    void startDateChanged();
    void endDateChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString taskName;
    QString description;
    QString bimElement;
    QString startDate;
    QString endDate;

};

Q_DECLARE_METATYPE(Task)

#endif // TASK_H

