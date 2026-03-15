#ifndef PERMISSION_CONTROLLER_H
#define PERMISSION_CONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QVariantList>

class PermissionController : public QObject
{
    Q_OBJECT
public:
    explicit PermissionController(QObject *parent = nullptr);

    Q_INVOKABLE void createPermission(int userId,
                                      QString entity,
                                      int projectId,
                                      bool canCreate,
                                      QString readType,
                                      bool canUpdate,
                                      QVariantList approverIds);
    // Q_INVOKABLE void getPermissionList();
    Q_INVOKABLE void getUserPermissionList(int userId, int projectId = -1);
    Q_INVOKABLE void updateUserPermission(int permissionId,
                                          int userId,
                                          int projectId,
                                          QString entity,
                                          QVariantList approverIds,
                                          bool canCreate,
                                          QString readType,
                                          bool canUpdate);

    Q_INVOKABLE void deletePermission(const QString& permissionId);

signals:
    void permissionListReceived(QVariantList permissionList);
    void permissionOperationSuccess(const QString& message);
    void permissionOperationFailed(const QString& error);

private:
    QString getAuthToken();
};

#endif
// PERMISSION_CONTROLLER_H
