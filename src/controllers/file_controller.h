#ifndef FILE_CONTROLLER_H
#define FILE_CONTROLLER_H

#include <QObject>
#include "repositories/file_repository.h"
#include "repositories/draft_entity_repository.h"

class FileController: public QObject
{
    Q_OBJECT
public:
    explicit FileController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &description, const QString &fileUrl, const QString &fileLocalPath) const;
    Q_INVOKABLE void approvedCreate(const QString &description, const QString &fileUrl, const QString &fileLocalPath) const;

    Q_INVOKABLE std::vector<File*> getFileList(bool isApproved = false) const;
    Q_INVOKABLE QString copyFileToFusionHub(const QString &sourcePath) const;


private:
    FileRepository* m_fileRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // FILE_CONTROLLER_H

