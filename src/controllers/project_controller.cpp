#include "project_controller.h"
#include "common/repository_locator.h"
#include <QDir>
//#import QtQuick.LocalStorage as Sql
extern int gProjectId;

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
    if (database.isOpen())
    {
        database.close();
    }

    if (!dbManager->initializeDatabase(projectName)) {
        qDebug() << "Failed to initialize database";
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();

    QString ifcFileName = "file.ifc";
    QString filePath = dbManager->getProjectPath() +"\\" + ifcFileName; // Specify the full path
    QFile file(filePath);
    qDebug()<< "File path:" << filePath;
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.close();
    }
    else
    {
        qDebug() << "Error opening file:" << file.errorString();
    }

    Project project;

    //QUuid uuid = QUuid::createUuid();
    //QString randomId = uuid.toString();

    //project.setId(123);
    //Temporary projectId creation
    qint64 milliseconds = QDateTime::currentMSecsSinceEpoch();

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


    if(m_projectRepository->save(project))
    {
        qDebug()<<"Data Saved";
    }
    else
    {
        qDebug()<<"Data not Saved";
    }

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
    }

    qDebug() << "Database initialized successfully!";
    qDebug() << "Project path:" << dbManager->getProjectPath();
}
