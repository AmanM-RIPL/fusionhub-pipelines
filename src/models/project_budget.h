#ifndef PROJECT_BUDGET_H
#define PROJECT_BUDGET_H

#include <QString>
#include <QMetaType>
#include <QObject>

class ProjectBudget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString dollarValue READ getDollarValue WRITE setDollarValue NOTIFY dollarValueChanged)
    Q_PROPERTY(int budgetHeadId READ getBudgetHeadId WRITE setBudgetHeadId NOTIFY budgetHeadIdChanged)

public:
    explicit ProjectBudget(QObject* parent = nullptr): QObject(parent) {}
    ProjectBudget(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
             const QString &DollarValue,
             int budgetHeadId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    QString getDollarValue() const { return dollarValue; }
    int getBudgetHeadId() const { return budgetHeadId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setDollarValue(const QString& dollarValue) { this->dollarValue = dollarValue; }
    void setBudgetHeadId(int budgetHeadId) { this->budgetHeadId = budgetHeadId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void dollarValueChanged();
    void budgetHeadIdChanged();


private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    QString dollarValue;
    int budgetHeadId = 0;
};

Q_DECLARE_METATYPE(ProjectBudget)

#endif // PROJECT_BUDGET_H

