#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Material: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString materialName READ getMaterialName WRITE setMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)
    Q_PROPERTY(int nextApprovingUser READ getNextApprovingUser WRITE setNextApprovingUser NOTIFY nextApprovingUserChanged)
    Q_PROPERTY(int createdByUser READ getCreatedByUser WRITE setCreatedByUser NOTIFY createdByUserChanged)



public:
    explicit Material(QObject* parent = nullptr): QObject(parent) {}
    Material(int id, const QString& globalId, const QString& approvalStatus,
             const QString& materialName, const QString& category,
             int unitOfMeasurementId,int nextApprovingUser, int createdByUser, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    QString getApprovalStatus() const { return approvalStatus; }
    QString getMaterialName() const { return materialName; }
    QString getCategory() const { return category; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }
    int getNextApprovingUser() const { return nextApprovingUser; }
    int getCreatedByUser() const { return createdByUser; }

    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(const QString& status) { this->approvalStatus = status; }
    void setMaterialName(const QString& materialName) { this->materialName = materialName; }
    void setCategory(const QString& category) { this->category = category; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }
    void setNextApprovingUser(int nextApprovingUser) { this->nextApprovingUser = nextApprovingUser; }
    void setCreatedByUser(int createdByUser) { this->createdByUser = createdByUser; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void materialNameChanged();
    void categoryChanged();
    void unitOfMeasurementIdChanged();
    void nextApprovingUserChanged();
    void createdByUserChanged();
private:
    int id = 0;
    QString globalId;
    QString approvalStatus;
    QString materialName;
    QString category;
    int unitOfMeasurementId = 0;
    int nextApprovingUser = 0;
    int createdByUser = 0;
};

Q_DECLARE_METATYPE(Material)

#endif // MATERIAL_H
