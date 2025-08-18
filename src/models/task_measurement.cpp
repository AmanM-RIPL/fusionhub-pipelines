#include "models/task_measurement.h"

TaskMeasurement::TaskMeasurement(int id, const QString& globalId, bool approvalStatus,
                                int taskId, const QDate& date, double length, int lengthUnitId,
                                double width, int widthUnitId, double height, int heightUnitId,
                                double diameter, int diameterUnitId, double volume, int volumeUnitId,
                                double surfaceArea, int surfaceAreaUnitId)
    : id(id), globalId(globalId), approvalStatus(approvalStatus), taskId(taskId), date(date),
      length(length), lengthUnitId(lengthUnitId), width(width), widthUnitId(widthUnitId),
      height(height), heightUnitId(heightUnitId), diameter(diameter), diameterUnitId(diameterUnitId),
      volume(volume), volumeUnitId(volumeUnitId), surfaceArea(surfaceArea), surfaceAreaUnitId(surfaceAreaUnitId)
{
}
