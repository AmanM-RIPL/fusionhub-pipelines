#include "project_repository.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>

extern QString gEnvironmentPath;
/*ProjectRepository::ProjectRepository()
{

}*/

ProjectRepository::ProjectRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<Project> ProjectRepository::findById(int id)
{
    return nullptr;
}
std::vector<std::unique_ptr<Project>> ProjectRepository::findAll()
{
    return {};
}
bool ProjectRepository::save(const Project& entity)
{   
    //QString folderPath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\";

    QDir dir;
    if (!dir.mkpath(gEnvironmentPath)) {
        qDebug() << "Failed to create project folder:" << gEnvironmentPath;
        return false;
    }
    QString filePath = gEnvironmentPath + "\\" + "main.json";
    createJsonFileAndAppendJsonObject(filePath, entity);
}

int ProjectRepository::getLastSyncedOn(int projectId)
{
     int lastSyncedOn = 0;

    QDir dir;
    if (!dir.mkpath(gEnvironmentPath)) {
        qDebug() << "Failed to update lastSyncedOn in project folder:" << gEnvironmentPath;
        return false;
    }
    QString filePath = gEnvironmentPath + "\\" + "main.json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return 0;
    }

    QByteArray rawData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);

    if (!doc.isArray()) {
        qDebug() << "JSON is not an array!";
        file.close();
        return 0;
    }
    QJsonArray jsonArray = doc.array();
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject rootObj = jsonArray[i].toObject();
        if (rootObj["id"].toInt() == projectId) {
            lastSyncedOn = rootObj["lastSyncedOn"].toInt();
            break;
        }
    }

    file.close();

     return lastSyncedOn;
}

void ProjectRepository::updateLastSyncedOn(int projectId, int lastSynced)
{
    QDir dir;
    if (!dir.mkpath(gEnvironmentPath)) {
        qDebug() << "Failed to update lastSyncedOn in project folder:" << gEnvironmentPath;
        return;
    }
    QString filePath = gEnvironmentPath + "\\" + "main.json";

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    QByteArray rawData = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(rawData);

    if (!doc.isArray()) {
        qDebug() << "JSON is not an array!";
        file.close();
        return;
    }
    QJsonArray jsonArray = doc.array();
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonObject rootObj = jsonArray[i].toObject();
        if (rootObj["id"].toInt() == projectId) {
            rootObj["lastSyncedOn"] = lastSynced;
            jsonArray.replace(i, rootObj);
            break;
        }
    }

    file.resize(0);
    file.write(QJsonDocument(jsonArray).toJson());
    file.close();
}


QString ProjectRepository::getProjectListAsJsonString(bool isBlocked)
{
    //QString folderPath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\";

    QDir dir;
    if (!dir.mkpath(gEnvironmentPath)) {
       // qDebug() << "Failed to create project folder:" << gEnvironmentPath;
        return "";
    }

    QString filePath = gEnvironmentPath + "\\" + "main.json";
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug() << "Failed to open JSON file:" << file.errorString();
        return "";
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonParseError parseError;
     QJsonDocument jsonArrayDoc = QJsonDocument::fromJson(jsonData, &parseError);
    if (jsonArrayDoc.isArray()) {
        QJsonArray jsonArray = jsonArrayDoc.array();       
        for(int i = jsonArray.count()-1; i >= 0; --i){
             QJsonObject element = jsonArray.at(i).toObject();
            if(element.value("isBlocked") != isBlocked)
            {
                jsonArray.removeAt(i);
                qDebug() << "removed at: " << i;
            }
        }

      jsonArrayDoc.setArray(jsonArray);
      jsonData = jsonArrayDoc.toJson(QJsonDocument::Compact);
    }
    else {
        qDebug() << "JSON data is not a root array.";
    }

    return QString(jsonData);
}

bool ProjectRepository::update(const Project& entity)
{
    return false;
}
bool ProjectRepository::deleteById(int id)
{
    return false;
}
std::vector<std::unique_ptr<Project>> ProjectRepository::findByApprovalStatus(bool status)
{
    return {};
}
QString ProjectRepository::getTableName() const
{
    return "Project";
}
std::unique_ptr<Project> ProjectRepository::mapFromQuery(const QSqlQuery& query) const
{
    return nullptr;
}
void ProjectRepository::bindEntityToQuery(QSqlQuery& query, const Project& entity) const
{
    /*
   // query.addBindValue(entity.getId());
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getProjectName());
    query.addBindValue(entity.getCustomerName());
    query.addBindValue(entity.getContactName());
    query.addBindValue(entity.getPhoneNumber());
    query.addBindValue(entity.getEmailId());
    query.addBindValue(entity.getTotalDollarValue());
    query.addBindValue(entity.getDescription());
    query.addBindValue(entity.getIsBlocked());
   */

}
QString ProjectRepository::getInsertQuery() const
{
    //return "INSERT INTO Project (id, global_id, approval_status, projectname, customername, contactname,"
    //       "phonenumber, emailid, totaldollarvalue, description, isBlocked) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";


    return "INSERT INTO Project (global_id, approval_status, projectname, customername, contactname,"
           "phonenumber, emailid, totaldollarvalue, description, isBlocked, lastSyncedOn) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";


}
QString ProjectRepository::getUpdateQuery() const
{
    return "";
}

void ProjectRepository::createJsonFileAndAppendJsonObject(const QString& filePath, const Project& entity)
{
    QJsonObject jsonObject;
    jsonObject.insert("id", entity.getId());
    jsonObject.insert("globalId", entity.getGlobalId());
    jsonObject.insert("approvalStatus", entity.getApprovalStatus());
    jsonObject.insert("projectName", entity.getProjectName());
    jsonObject.insert("customerName", entity.getCustomerName());
    jsonObject.insert("contactName", entity.getContactName());
    jsonObject.insert("phoneNumber", entity.getPhoneNumber());
    jsonObject.insert("emailid", entity.getEmailId());
    jsonObject.insert("totalDollarValue", entity.getTotalDollarValue());
    jsonObject.insert("isBlocked", entity.getIsBlocked());
    jsonObject.insert("description", entity.getDescription());
    jsonObject.insert("lastSyncedOn", entity.getLastSyncedOn());

    QDate currentDate = QDate::currentDate();
    jsonObject.insert("dateData", currentDate.toString(Qt::ISODate));

    QFile file(filePath);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "Failed to open file for reading/writing:" << file.errorString();
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument doc;
    if (!jsonData.isEmpty()) {
        doc = QJsonDocument::fromJson(jsonData);
        if (doc.isNull()) {
            qDebug() << "Failed to parse JSON from file.";
            return;
        }
    }

    QJsonArray jsonArray;
    if (doc.isArray()) {
        jsonArray = doc.array();
    } else if (!doc.isNull()) {
        // If the file exists but is not an array, you might want to handle this case
        // e.g., wrap existing content in an array, or throw an error.
        qDebug() << "Existing JSON is not an array. Creating a new array.";
    }

    jsonArray.append(jsonObject);
    doc = QJsonDocument(jsonArray);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return;
    }

    file.write(doc.toJson(QJsonDocument::Indented)); // Use Indented for readability
    file.close();
}
