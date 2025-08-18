// Stub implementations for remaining models

#ifndef FILE_H
#define FILE_H
#include <QString>
#include <QMetaType>
class File {
public:
    File() = default;
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getDescription() const { return description; }
    QString getFileUrl() const { return fileUrl; }
    QString getFileLocalPath() const { return fileLocalPath; }
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setDescription(const QString& description) { this->description = description; }
    void setFileUrl(const QString& fileUrl) { this->fileUrl = fileUrl; }
    void setFileLocalPath(const QString& fileLocalPath) { this->fileLocalPath = fileLocalPath; }
private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString description;
    QString fileUrl;
    QString fileLocalPath;
};
Q_DECLARE_METATYPE(File)
#endif
