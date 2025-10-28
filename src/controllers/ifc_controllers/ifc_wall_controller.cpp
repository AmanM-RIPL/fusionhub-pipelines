#include "ifc_wall_controller.h"
#include "common/repository_locator.h"
#include <QDir>

IFCWallController::IFCWallController(QObject *parent)
    : QObject{parent},
    m_ifcWallRepository(RepositoryLocator::instance().ifcWallRepository())
{}


void IFCWallController::create(const QString &projectName) const
{
    /*
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
  */
    IFCWall ifcWall;

    m_ifcWallRepository->save(ifcWall);
}
