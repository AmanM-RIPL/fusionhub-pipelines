#include "project_controller.h"
#include "network/network_manager.h"
#include "common/repository_locator.h"
#include <QDir>
//#import QtQuick.LocalStorage as Sql
#include "ifc_detail_controller.h"

extern int gProjectId;
extern QString gProjectName;

ProjectController::ProjectController(QObject *parent)
    : QObject{parent},
   m_projectRepository(RepositoryLocator::instance().projectRepository())   
{}

void ProjectController::create(const QString &projectName, const QString &customerName,
                               const QString &contactName, const QString &phoneNumber,
                               const QString &emailId, const QString &totalDollarValue,
                               const QString &description) const
{
    qInfo() << projectName << " " << customerName << " " <<contactName<< ""
            << phoneNumber << " " << totalDollarValue << " " <<description;

    auto dbManager = DatabaseManager::getInstance();

    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen()){
        database.close();
    }

    if (!dbManager->initializeDatabase(projectName)) {
        qDebug() << "Failed to initialize database";
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    //QUuid uuid = QUuid::createUuid();
    //QString randomId = uuid.toString();
    //project.setId(123);
    //Temporary projectId creation
    qint64 milliseconds = QDateTime::currentMSecsSinceEpoch();

    Project project;
    project.setId(milliseconds);
    project.setGlobalId("123");
    project.setApprovalStatus(true);
    project.setProjectName(projectName);
    project.setCustomerName(customerName);
    project.setContactName(contactName);
    project.setPhoneNumber(phoneNumber);
    project.setEmailId(emailId);
    project.setTotalDollarValue(totalDollarValue);
    project.setDescription(description);
    project.setIsBlocked(false);
    project.setLastSyncedOn(milliseconds);
    project.setLastChangeLogId(1);

    if(m_projectRepository->save(project)){
        qDebug()<<"Data Saved";
    }
    else{
        qDebug()<<"Data not Saved";
    }


    IFCDetail ifcDetail;
    ifcDetail.setGlobalId("123");   
    ifcDetail.setName(projectName);
    ifcDetail.setType("NA");

    IFCDetailController ifcController;
    bool bResult =  ifcController.create(projectName, ifcDetail);
    if(bResult){
        qDebug()<<"ifc file created successfully";
    }
    else{
         qDebug()<<"ifc file could not be created";
    }
}

void ProjectController::getAllProjectList(bool isBlocked)
{
    QString token = NetworkManager::getInstance()->getStoredToken();
    if(token.isEmpty()) {
        emit projectListFailed("Please login first");
        return;
    }

    QUrl url("http://127.0.0.1:8080/api/v1/default/project");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if(reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(responseData);

            if(doc.isArray()) {
                QJsonArray array = doc.array();
                QVariantList list;
                for(const QJsonValue &value : array) {
                    QJsonObject obj = value.toObject();
                    QVariantMap project;

                    project["id"] = obj["id"].toInt();
                    project["projectName"] = obj["projectName"].toString();
                    project["status"] = obj["status"].toString();
                    project["description"] = obj["description"].toString();
                    project["customerName"] = obj["customerName"].toString();
                    project["startDate"] = obj["startDate"].toString();
                    project["endDate"] = obj["endDate"].toString();

                    list.append(project);
                }
                emit projectListReceived(list);
            } else {
                emit projectListFailed("Invalid Project JSON format");
            }
        } else {
            emit projectListFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

QString ProjectController::getProjectList(bool isBlocked) const
{
    QString str = m_projectRepository->getProjectListAsJsonString(isBlocked);
    return str;
}

void ProjectController::openDatabase(const QString &projectName, int projectId)const
{
    auto dbManager = DatabaseManager::getInstance();

    QSqlDatabase database = dbManager->getDatabase();
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase(projectName)) {
        qDebug() << "Failed to initialize database";
    }
    else{
        gProjectId = projectId;
        gProjectName = projectName;
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();
}

int ProjectController::getLastSyncedOn()const
{
    int lastSyncedOn = m_projectRepository->getLastSyncedOn(gProjectId);
    return lastSyncedOn;
}

void ProjectController::updateLastSyncedOn(int lastSyncedOn)const
{
    m_projectRepository->updateLastSyncedOn(gProjectId, lastSyncedOn);
}

int ProjectController::getLastChangeLogId()const
{
    int lastChangeLogId = m_projectRepository->getLastChangeLogId(gProjectId);
    return lastChangeLogId;
}

void ProjectController::updateLastChangeLogId(int lastChangeLogId)const
{
    m_projectRepository->updateLastChangeLogId(gProjectId, lastChangeLogId);
}

