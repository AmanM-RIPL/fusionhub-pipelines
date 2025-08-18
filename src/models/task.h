#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QMetaType>

class Task
{
public:
    Task() = default;
    Task(int id, const QString& globalId, bool approvalStatus,
         const QString& taskName, const QString& description,
         const QString& bimElement, const QDate& startDate, const QDate& endDate);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getTaskName() const { return taskName; }
    QString getDescription() const { return description; }
    QString getBimElement() const { return bimElement; }
    QDate getStartDate() const { return startDate; }
    QDate getEndDate() const { return endDate; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setTaskName(const QString& taskName) { this->taskName = taskName; }
    void setDescription(const QString& description) { this->description = description; }
    void setBimElement(const QString& bimElement) { this->bimElement = bimElement; }
    void setStartDate(const QDate& startDate) { this->startDate = startDate; }
    void setEndDate(const QDate& endDate) { this->endDate = endDate; }

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString taskName;
    QString description;
    QString bimElement;
    QDate startDate;
    QDate endDate;
};

Q_DECLARE_METATYPE(Task)

#endif // TASK_H
