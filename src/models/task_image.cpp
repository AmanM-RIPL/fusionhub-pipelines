#include "models/task_image.h"

TaskImage::TaskImage(int id, const QString& globalId, bool approvalStatus,
                     int taskId, const QString& imageUrl, const QString& imageLocalPath)
    : id(id), globalId(globalId), approvalStatus(approvalStatus),
      taskId(taskId), imageUrl(imageUrl), imageLocalPath(imageLocalPath)
{
}
