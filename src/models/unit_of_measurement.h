#ifndef UNIT_OF_MEASUREMENT_H
#define UNIT_OF_MEASUREMENT_H

#include <QString>
#include <QMetaType>
#include <QObject>

class UnitOfMeasurement: public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString uomName READ getUomName WRITE setUomName NOTIFY uomNameChanged)
    Q_PROPERTY(QString unitType READ getUnitType WRITE setUnitType NOTIFY unitTypeChanged)
    Q_PROPERTY(double conversionToSqm READ getConversionToSqm WRITE setConversionToSqm NOTIFY conversionToSqmChanged)
    Q_PROPERTY(double conversionToCubicMeter READ getConversionToCubicMeter WRITE setConversionToCubicMeter NOTIFY conversionToCubicMeterChanged)
    Q_PROPERTY(double conversionToMeter READ getConversionToMeter WRITE setConversionToMeter NOTIFY setConversionToMeter)
    Q_PROPERTY(double conversionToKilogram READ getConversionToKilogram WRITE setConversionToKilogram NOTIFY setConversionToKilogram)

public:
    explicit UnitOfMeasurement(QObject* parent = nullptr): QObject(parent) {}
    UnitOfMeasurement(int id, const QString& globalId, bool approvalStatus,
                     const QString& uomName, const QString& unitType,
                     double conversionToSqm = 0.0, double conversionToCubicMeter = 0.0,
                     double conversionToMeter = 0.0, double conversionToKilogram = 0.0, QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getUomName() const { return uomName; }
    QString getUnitType() const { return unitType; }
    double getConversionToSqm() const { return conversionToSqm; }
    double getConversionToCubicMeter() const { return conversionToCubicMeter; }
    double getConversionToMeter() const { return conversionToMeter; }
    double getConversionToKilogram() const { return conversionToKilogram; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setUomName(const QString& uomName) { this->uomName = uomName; }
    void setUnitType(const QString& unitType) { this->unitType = unitType; }
    void setConversionToSqm(double conversion) { this->conversionToSqm = conversion; }
    void setConversionToCubicMeter(double conversion) { this->conversionToCubicMeter = conversion; }
    void setConversionToMeter(double conversion) { this->conversionToMeter = conversion; }
    void setConversionToKilogram(double conversion) { this->conversionToKilogram = conversion; }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void uomNameChanged();
    void unitTypeChanged();
    void conversionToSqmChanged();
    void conversionToCubicMeterChanged();
    void conversionToMeterChanged();
    void conversionToKilogramChanged();


private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString uomName;
    QString unitType;
    double conversionToSqm = 0.0;
    double conversionToCubicMeter = 0.0;
    double conversionToMeter = 0.0;
    double conversionToKilogram = 0.0;
};

Q_DECLARE_METATYPE(UnitOfMeasurement)

#endif
