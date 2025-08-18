#ifndef MATERIAL_H
#define MATERIAL_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Material: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString materialName READ getMaterialName WRITE setMaterialName NOTIFY materialNameChanged)
    Q_PROPERTY(QString category READ getCategory WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(int unitOfMeasurementId READ getUnitOfMeasurementId WRITE setUnitOfMeasurementId NOTIFY unitOfMeasurementIdChanged)

public:
    explicit Material(QObject* parent = nullptr): QObject(parent) {}
    Material(int id, const QString& globalId, bool approvalStatus,
             const QString& materialName, const QString& category,
             int unitOfMeasurementId, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getMaterialName() const { return materialName; }
    QString getCategory() const { return category; }
    int getUnitOfMeasurementId() const { return unitOfMeasurementId; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setMaterialName(const QString& materialName) { this->materialName = materialName; }
    void setCategory(const QString& category) { this->category = category; }
    void setUnitOfMeasurementId(int unitOfMeasurementId) { this->unitOfMeasurementId = unitOfMeasurementId; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void materialNameChanged();
    void categoryChanged();
    void unitOfMeasurementIdChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString materialName;
    QString category;
    int unitOfMeasurementId = 0;
};

Q_DECLARE_METATYPE(Material)

#endif // MATERIAL_H
