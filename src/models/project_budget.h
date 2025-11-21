#ifndef PROJECT_BUDGET_H
#define PROJECT_BUDGET_H

#include <QString>
#include <QMetaType>
#include <QObject>

class ProjectBudget: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString dollarValue READ getDollarValue WRITE setDollarValue NOTIFY dollarValueChanged)
    Q_PROPERTY(int budgetHeadId READ getBudgetHeadId WRITE setBudgetHeadId NOTIFY budgetHeadIdChanged)

public:
    explicit ProjectBudget(QObject* parent = nullptr): QObject(parent) {}
    ProjectBudget(int id, const QString& globalId, bool approvalStatus,
             const QString &DollarValue,
             int budgetHeadId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getDollarValue() const { return dollarValue; }
    int getBudgetHeadId() const { return budgetHeadId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setDollarValue(const QString& dollarValue) { this->dollarValue = dollarValue; }
    void setBudgetHeadId(int budgetHeadId) { this->budgetHeadId = budgetHeadId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void dollarValueChanged();
    void budgetHeadIdChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString dollarValue;
    int budgetHeadId = 0;
};

Q_DECLARE_METATYPE(ProjectBudget)

#endif // PROJECT_BUDGET_H

