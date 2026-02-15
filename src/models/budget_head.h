#ifndef BUDGET_HEAD_H
#define BUDGET_HEAD_H

#include <QString>
#include <QMetaType>
#include <QObject>

class BudgetHead : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)


public:
    explicit BudgetHead(QObject* parent = nullptr): QObject(parent) {}
    BudgetHead(int id, const QString& globalId, const QString& approvalStatus, const QString& description,int nextApprovingUser, int createdByUser, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    QString getDescription() const { return description; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setDescription(const QString& description) { this->description = description; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void descriptionChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();

private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    QString description;
    int nextApprovingUser = 0;
    int createdByUser = 0;
};

Q_DECLARE_METATYPE(BudgetHead)

#endif // BUDGET_HEAD_H
