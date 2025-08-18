#ifndef BUDGET_HEAD_H
#define BUDGET_HEAD_H

#include <QString>
#include <QMetaType>
#include <QObject>

class BudgetHead : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)

public:
    explicit BudgetHead(QObject* parent = nullptr): QObject(parent) {}
    BudgetHead(int id, const QString& globalId, bool approvalStatus, const QString& description, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getDescription() const { return description; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setDescription(const QString& description) { this->description = description; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void descriptionChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString description;
};

Q_DECLARE_METATYPE(BudgetHead)

#endif // BUDGET_HEAD_H
