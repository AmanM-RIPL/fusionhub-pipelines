#include "permission_controller.h"
#include "user_controller.h"
#include "network/network_manager.h"
#include <QDebug>

PermissionController::PermissionController(QObject *parent) : QObject(parent) {}

QString PermissionController::getAuthToken()
{
    return NetworkManager::getInstance()->getStoredToken();
}
void PermissionController::createPermission(int userId,
                                            QString entity,
                                            int projectId,
                                            bool canCreate,
                                            QString readType,
                                            bool canUpdate,
                                            QVariantList approverIds)
{
    QString token = getAuthToken();
    if (token.isEmpty()) {
        emit permissionOperationFailed("Authorization token is missing.");
        return;
    }

    QUrl url("http://127.0.0.1:8080/api/v1/default/permission");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    // 1. Root Object
    QJsonObject root;
    root["user"] = userId;
    root["project"] = projectId;
    root["entity"] = entity;
    root["tenant"] = 1;
    root["permission_type"] = "CREATE";

    root["approval"] = QJsonArray::fromVariantList(approverIds);

    // 2. Nested Filter Object
    QJsonObject filterObj;
    QJsonArray projectFilterArray;
    projectFilterArray.append(projectId);
    filterObj["project"] = projectFilterArray;
    root["filter"] = filterObj;

    // 3. Nested Access Object
    QJsonObject accessObj;
    accessObj["create"] = canCreate;
    accessObj["read"] = readType;
    accessObj["update"] = canUpdate;
    root["access"] = accessObj;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QJsonDocument doc(root);
    QByteArray jsonData = doc.toJson();

    qDebug() << "Final JSON being sent:" << jsonData;

    QNetworkReply* reply = manager->post(request, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Permission Created Successfully:" << reply->readAll();
            emit permissionOperationSuccess("Permission Created Successfully");
        } else {
            qDebug() << "API Error:" << reply->errorString();
            emit permissionOperationFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void PermissionController::updateUserPermission(int permissionId,
                                                int userId,
                                                int projectId,
                                                QString entity,
                                                QVariantList approverIds,
                                                bool canCreate,
                                                QString readType,
                                                bool canUpdate)
{
    QString token = getAuthToken();
    if (token.isEmpty()) return;
    QUrl url(QString("http://127.0.0.1:8080/api/v1/default/permission/%1").arg(permissionId));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QJsonObject root;
    root["user"] = userId;
    root["project"] = projectId;
    root["entity"] = entity;
    root["approval"] = QJsonArray::fromVariantList(approverIds);

    QJsonObject accessObj;
    accessObj["create"] = canCreate;
    accessObj["read"] = readType;
    accessObj["update"] = canUpdate;
    root["access"] = accessObj;

    // Filter logic
    QJsonObject filterObj;
    QJsonArray projArray; projArray.append(projectId);
    filterObj["project"] = projArray;
    root["filter"] = filterObj;

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);

    //  QNetworkReply* reply = manager->patch(request, QJsonDocument(root).toJson());
    QByteArray data = QJsonDocument(root).toJson();

    QNetworkReply *reply = manager->sendCustomRequest( request,"PATCH", data );

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit permissionOperationSuccess("Permission Updated Successfully");
        } else {
            emit permissionOperationFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void PermissionController::getUserPermissionList(int userId, int projectId)
{
    QString token = getAuthToken();
    UserController *userController = new UserController(this);

    connect(userController, &UserController::userListReceived, this, [=](QVariantList userList) {

        QMap<int, QString> userMap;
        for (const QVariant &item : userList) {
            QVariantMap u = item.toMap();
            userMap[u["id"].toInt()] = u["firstName"].toString() + " " + u["lastName"].toString();
        }
        userController->deleteLater();

        QUrl url("http://127.0.0.1:8080/api/v1/default/permission/byUserAndProjectId");
        QUrlQuery query;
        query.addQueryItem("userId", QString::number(userId));

        if (projectId > 0) {
            query.addQueryItem("projectId", QString::number(projectId));
        }
        url.setQuery(query);

        QNetworkRequest request(url);
        request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

        QNetworkAccessManager* manager = new QNetworkAccessManager(this);
        QNetworkReply* reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, this, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
                QVariantList uiList;

                if (doc.isArray()) {
                    QJsonArray array = doc.array();
                    for (const QJsonValue &value : array) {
                        QJsonObject obj = value.toObject();
                        QVariantMap map;

                        // Basic fields mapping
                        map["id"]        = obj["id"].toInt();
                        map["entity"]    = obj["entity"].toString();
                        map["project"]   = obj["project"].toInt();
                        map["userId"]    = obj["user"].toInt();
                        map["createdOn"] = obj["createdOn"].toString();

                        // User Name for the main user
                        int mainUserId = obj["user"].toInt();
                        map["userName"] = userMap.value(mainUserId, "Unknown");

                        // --- Approval Names Mapping Start ---
                        QJsonArray approvalIds = obj["approval"].toArray();
                        QVariantList approvalNamesList;

                        for (const QJsonValue &appIdVal : approvalIds) {
                            int appId = appIdVal.toInt();

                            QString appUserName = userMap.value(appId, QString("User %1").arg(appId));

                            QVariantMap appUserObj;
                            appUserObj["id"] = appId;
                            appUserObj["name"] = appUserName;

                            approvalNamesList.append(appUserObj);
                        }

                        map["approvalData"] = approvalNamesList;
                        // --- Approval Names Mapping End ---

                        // Access Parsing
                        QJsonObject access = obj["access"].toObject();
                        map["canCreate"] = access["create"].toBool();
                        map["readType"]  = access["read"].toString();
                        map["canUpdate"] = access["update"].toBool();

                        uiList.append(map);
                    }
                }
                emit permissionListReceived(uiList);
            } else {
                qDebug() << "API Error:" << reply->errorString();
            }
            reply->deleteLater();
            manager->deleteLater();
        });
    });

    userController->getUserList();
}

// DELETE
void PermissionController::deletePermission(const QString& permissionId)
{
    QString token = getAuthToken();
    QUrl url(QString("%1/%2").arg("http://127.0.0.1:8080/api/v1/default/permission").arg(permissionId));

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QString("Bearer %1").arg(token).toUtf8());

    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    QNetworkReply* reply = manager->deleteResource(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if (reply->error() == QNetworkReply::NoError) {
            emit permissionOperationSuccess("Permission Deleted");
            // getPermissionList();
        } else {
            // emit permissionOperationFailed(reply->errorString());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}
