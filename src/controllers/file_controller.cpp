#include "file_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

FileController::FileController(QObject *parent)
    : QObject{parent},
    m_fileRepository(RepositoryLocator::instance().fileRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void FileController::create(const QString &description, const QString &fileUrl, const QString &fileLocalPath) const
{

    /***************** DraftEntity Start *****************/

    QJsonObject jsonObject;
    jsonObject["description"] = description;
    jsonObject["fileUrl"] = fileUrl;
    jsonObject["fileLocalPath"] = fileLocalPath;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);

    qDebug() << "File::EntitySchema: " << entitySchema;

    // change history
    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoTime = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoTime;
    jsonObjectChangeHistory["changeType"] = "create";
    jsonObjectChangeHistory["description"] = "Created File Entity";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(QDate::currentDate());
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("File");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

void FileController::approvedCreate(const QString &description, const QString &fileUrl, const QString &fileLocalPath) const
{
    File file;

    file.setId(0);
    file.setGlobalId("123");
    file.setApprovalStatus(true);

    file.setDescription(description);
    file.setFileUrl(fileUrl);
    file.setFileLocalPath(fileLocalPath);

    m_fileRepository->saveQML(&file);
}

std::vector<File*> FileController::getFileList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    if (isApproved) {
        return m_fileRepository->findAllQML();
    } else {
        std::vector<DraftEntity*> drafts = m_draftEntityRepository->findAllQML("File");
        std::vector<File*> files;

        for (int i = 0; i < drafts.size(); i++) {

            QString jsonString = drafts[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

            if (!jsonDoc.isNull() && jsonDoc.isObject()) {

                auto file = new File();
                QJsonObject jsonObj = jsonDoc.object();

                file->setId(i + 1);
                file->setGlobalId("draft");
                file->setApprovalStatus(false);

                file->setDescription(jsonObj["description"].toString());
                file->setFileUrl(jsonObj["fileUrl"].toString());
                file->setFileLocalPath(jsonObj["fileLocalPath"].toString());

                files.push_back(file);
            }
        }
        return files;
    }
}

QString FileController::copyFileToFusionHub(const QString &sourcePath) const
{
    QString targetDir = "C:/Users/RIPL/Documents/FusionHubData/NMBPL/";

    QDir dir;
    if (!dir.exists(targetDir)) {
        dir.mkpath(targetDir);
    }

    QFileInfo fileInfo(sourcePath);
    QString fileName = fileInfo.fileName();
    QString targetPath = targetDir + "/" + fileName;

    // Overwrite if exists
    if (QFile::exists(targetPath)) {
        QFile::remove(targetPath);
    }

    // Copy file
    if (QFile::copy(sourcePath, targetPath)) {
        qDebug() << "File copied to:" << targetPath;
        return targetPath;
    } else {
        qDebug() << "File copy failed:" << sourcePath;
        return "";
    }
}


