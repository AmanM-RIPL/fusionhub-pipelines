#ifndef TASK_MEASUREMENT_H
#define TASK_MEASUREMENT_H

#include <QString>
#include <QDate>
#include <QMetaType>

class TaskMeasurement
{
public:
    TaskMeasurement() = default;
    TaskMeasurement(int id, const QString& globalId, bool approvalStatus,
                   int taskId, const QDate& date, double length, int lengthUnitId,
                   double width, int widthUnitId, double height, int heightUnitId,
                   double diameter, int diameterUnitId, double volume, int volumeUnitId,
                   double surfaceArea, int surfaceAreaUnitId);
    
    // Getters
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getTaskId() const { return taskId; }
    QDate getDate() const { return date; }
    double getLength() const { return length; }
    int getLengthUnitId() const { return lengthUnitId; }
    double getWidth() const { return width; }
    int getWidthUnitId() const { return widthUnitId; }
    double getHeight() const { return height; }
    int getHeightUnitId() const { return heightUnitId; }
    double getDiameter() const { return diameter; }
    int getDiameterUnitId() const { return diameterUnitId; }
    double getVolume() const { return volume; }
    int getVolumeUnitId() const { return volumeUnitId; }
    double getSurfaceArea() const { return surfaceArea; }
    int getSurfaceAreaUnitId() const { return surfaceAreaUnitId; }
    
    // Setters
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setDate(const QDate& date) { this->date = date; }
    void setLength(double length) { this->length = length; }
    void setLengthUnitId(int lengthUnitId) { this->lengthUnitId = lengthUnitId; }
    void setWidth(double width) { this->width = width; }
    void setWidthUnitId(int widthUnitId) { this->widthUnitId = widthUnitId; }
    void setHeight(double height) { this->height = height; }
    void setHeightUnitId(int heightUnitId) { this->heightUnitId = heightUnitId; }
    void setDiameter(double diameter) { this->diameter = diameter; }
    void setDiameterUnitId(int diameterUnitId) { this->diameterUnitId = diameterUnitId; }
    void setVolume(double volume) { this->volume = volume; }
    void setVolumeUnitId(int volumeUnitId) { this->volumeUnitId = volumeUnitId; }
    void setSurfaceArea(double surfaceArea) { this->surfaceArea = surfaceArea; }
    void setSurfaceAreaUnitId(int surfaceAreaUnitId) { this->surfaceAreaUnitId = surfaceAreaUnitId; }

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int taskId = 0;
    QDate date;
    double length = 0.0;
    int lengthUnitId = 0;
    double width = 0.0;
    int widthUnitId = 0;
    double height = 0.0;
    int heightUnitId = 0;
    double diameter = 0.0;
    int diameterUnitId = 0;
    double volume = 0.0;
    int volumeUnitId = 0;
    double surfaceArea = 0.0;
    int surfaceAreaUnitId = 0;
};

Q_DECLARE_METATYPE(TaskMeasurement)

#endif // TASK_MEASUREMENT_H
