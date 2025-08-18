#ifndef TASK_IMAGE_H
#define TASK_IMAGE_H

#include <QString>
#include <QMetaType>

class TaskImage
{
public:
    TaskImage() = default;
    TaskImage(int id, const QString& globalId, bool approvalStatus,
              int taskId, const QString& imageUrl, const QString& imageLocalPath);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    int getTaskId() const { return taskId; }
    QString getImageUrl() const { return imageUrl; }
    QString getImageLocalPath() const { return imageLocalPath; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setTaskId(int taskId) { this->taskId = taskId; }
    void setImageUrl(const QString& imageUrl) { this->imageUrl = imageUrl; }
    void setImageLocalPath(const QString& imageLocalPath) { this->imageLocalPath = imageLocalPath; }

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    int taskId = 0;
    QString imageUrl;
    QString imageLocalPath;
};

Q_DECLARE_METATYPE(TaskImage)

#endif // TASK_IMAGE_H
