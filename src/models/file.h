#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QMetaType>
#include <QObject>

class File : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus NOTIFY approvalStatusChanged)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId NOTIFY globalIdChanged)
    Q_PROPERTY(QString description READ getDescription WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString fileUrl READ getFileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileLocalPath READ getFileLocalPath WRITE setFileLocalPath NOTIFY fileLocalPathChanged)

public:
    explicit File(QObject* parent = nullptr) : QObject(parent) {}

    File(int id,
         const QString& globalId,
         bool approvalStatus,
         const QString& description,
         const QString& fileUrl,
         const QString& fileLocalPath,
         QObject* parent = nullptr);

    // Getters
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getDescription() const { return description; }
    QString getFileUrl() const { return fileUrl; }
    QString getFileLocalPath() const { return fileLocalPath; }

    // Setters
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; emit globalIdChanged(); }
    void setApprovalStatus(bool status) { this->approvalStatus = status; emit approvalStatusChanged(); }
    void setDescription(const QString &description) { this->description = description; emit descriptionChanged(); }
    void setFileUrl(const QString &fileUrl) { this->fileUrl = fileUrl; emit fileUrlChanged(); }
    void setFileLocalPath(const QString &fileLocalPath) { this->fileLocalPath = fileLocalPath; emit fileLocalPathChanged(); }

signals:
    void globalIdChanged();
    void approvalStatusChanged();
    void descriptionChanged();
    void fileUrlChanged();
    void fileLocalPathChanged();

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;

    QString description;
    QString fileUrl;
    QString fileLocalPath;
};

Q_DECLARE_METATYPE(File)

#endif // FILE_H
