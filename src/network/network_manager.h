#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class NetworkManager : public QObject {
    Q_OBJECT

    Q_PROPERTY(int userId READ getUserId NOTIFY userIdChanged)

public:
    static NetworkManager* getInstance();

    Q_INVOKABLE void sendDraftToServer(const QJsonObject& payload);
    Q_INVOKABLE void requestChangeLogSync();

    // Getters
    QString getStoredToken() const;
    QJsonObject getLastUserData() const;
    int getLastStatusCode() const;
    bool isTokenValid() const;
    void clearData();
    int getUserId() const;

    // API Methods
    void loginAPI(const QString& username, const QString& password);

signals:
    void loginInProgress();
    void loginDone();
    void loginFailed(const QString& errorMessage);
    void responseReceived(const QByteArray& data);
    void changeLogSyncReceived(const QJsonArray& changeLogs);

    void userIdChanged();

private slots:
    void onChangeLogSyncFinished(QNetworkReply* reply);

private:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager();

    // Response handlers
    void handleLoginResponse(QNetworkReply* reply);
    void parseLoginResponse(const QJsonDocument& jsonDoc);

    QNetworkAccessManager* m_manager;
    QString m_authToken;
    QJsonObject m_lastUserData;
    int m_lastStatusCode;
    int m_userId = 0;
};

#endif // NETWORK_MANAGER_H
