#ifndef MATERIAL_INDENT_H
#define MATERIAL_INDENT_H
#include <QString>
#include <QMetaType>
#include <QObject>

class MaterialIndent: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(double quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(int taskId READ getTaskId WRITE setTaskId NOTIFY taskIdChanged)
    Q_PROPERTY(QString materialName READ getMaterialName WRITE setMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(QString taskName READ getTaskName WRITE setTaskName NOTIFY taskNameChanged)



public:
    explicit MaterialIndent(QObject* parent = nullptr): QObject(parent) {}

    MaterialIndent(int id, const QString& globalId, const QString& approvalStatus, int nextApprovingUser, int createdByUser,
                   const double quantity, int materialId, int taskId,
                   const QString& materialName, const QString& taskName,
                   QObject* parent = nullptr);

    // --- Getters ---
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }
    double getQuantity() const { return quantity; }
    int getMaterialId() const { return materialId; }
    int getTaskId() const { return taskId; }
    QString getMaterialName() const { return materialName; }
    QString getTaskName() const { return taskName; }

    // --- Setters ---
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }
    void setQuantity(const double quantity) { this->quantity = quantity; }
    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setMaterialName(const QString& materialName) { this->materialName = materialName; }
    void setTaskName(const QString& taskName) { this->taskName = taskName; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
    void quantityChanged();
    void materialIdChanged();
    void taskIdChanged();
    void materialNameChanged();
    void taskNameChanged();

private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    int nextApprovingUser = 0;
    int createdByUser = 0;
    double quantity = 0.0;
    int materialId = 0;
    int taskId = 0;
    QString materialName;
    QString taskName;
};

Q_DECLARE_METATYPE(MaterialIndent)
#endif // MATERIAL_INDENT_H
