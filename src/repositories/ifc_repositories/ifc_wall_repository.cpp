#include "ifc_wall_repository.h"

extern QString gEnvironmentPath;

IFCWallRepository::IFCWallRepository(QObject* parent) : QObject(parent) {
    setlocale(LC_TIME, "");
}

bool IFCWallRepository::save(const IFCWall& entity)
{
    QDir dir;
    if (!dir.mkpath(gEnvironmentPath)) {
        qDebug() << "Failed to create project folder:" << gEnvironmentPath;
        return false;
    }
    //QString filePath = gEnvironmentPath + "\\" + "main.json";
    //createJsonFileAndAppendJsonObject(filePath, entity);
    return true;
}
