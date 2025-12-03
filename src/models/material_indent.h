#ifndef MATERIAL_INDENT_H
#define MATERIAL_INDENT_H

#include <QString>
#include <QMetaType>
#include <QObject>

class MaterialIndent: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(double quantity READ getQuantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(int materialId READ getMaterialId WRITE setMaterialId NOTIFY materialIdChanged)
    Q_PROPERTY(int taskId READ getTaskId WRITE setTaskId NOTIFY taskIdChanged)

public:
    explicit MaterialIndent(QObject* parent = nullptr): QObject(parent) {}
    MaterialIndent(int id, const QString& globalId, bool approvalStatus,
             const double quantity, int materialId, int taskId, QObject* parent = nullptr);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    double getQuantity() const { return quantity; }
    int getMaterialId() const { return materialId; }
    int getTaskId() const { return taskId; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setQuantity(const double quantity) { this->quantity = quantity; }
    void setMaterialId(int materialId) { this->materialId = materialId; }
    void setTaskId(int taskId) { this->taskId = taskId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void quantityChanged();
    void materialIdChanged();
    void taskIdChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    double quantity;
    int materialId = 0;
    int taskId = 0;
};

Q_DECLARE_METATYPE(MaterialIndent)

#endif // MATERIAL_INDENT_H

