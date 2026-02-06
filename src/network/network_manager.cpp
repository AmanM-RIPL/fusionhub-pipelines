#include "network_manager.h"
#include "controllers/project_controller.h"
#include <QDebug>

// Static singleton instance
NetworkManager* NetworkManager::getInstance()
{
    static NetworkManager instance;
    return &instance;
}

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent),
    m_authToken(""),
    m_lastStatusCode(0)

{
    m_manager = new QNetworkAccessManager(this);
}

NetworkManager::~NetworkManager()
{
}

QString NetworkManager::getStoredToken() const
{
    return m_authToken;
}

QJsonObject NetworkManager::getLastUserData() const
{
    return m_lastUserData;
}
int NetworkManager::getUserId() const
{
    return m_userId;
}


int NetworkManager::getLastStatusCode() const
{
    return m_lastStatusCode;
}

bool NetworkManager::isTokenValid() const
{
    return !m_authToken.isEmpty();
}

void NetworkManager::clearData()
{
    m_authToken.clear();
    m_lastUserData = QJsonObject();
    m_lastStatusCode = 0;
}

void NetworkManager::loginAPI(const QString& username, const QString& password)
{
    if (username.isEmpty() || password.isEmpty()) {
        emit loginFailed("Username or password is empty");
        return;
    }
    emit loginInProgress();

    QUrl url("http://127.0.0.1:8080/api/v1/auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // JSON Payload
    QJsonObject payload;
    payload["username"] = username;
    payload["password"] = password;
    QByteArray data = QJsonDocument(payload).toJson();

    QNetworkReply* reply = m_manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleLoginResponse(reply);
    });
}

void NetworkManager::handleLoginResponse(QNetworkReply* reply)
{
    m_lastStatusCode = reply->attribute(
                                QNetworkRequest::HttpStatusCodeAttribute).toInt();

    QByteArray responseData = reply->readAll();
    emit responseReceived(responseData);

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Network error:" << reply->errorString();
        emit loginFailed(reply->errorString());
        reply->deleteLater();
        return;
    }

    // Check HTTP status code
    if (m_lastStatusCode != 200) {
        QString errorMsg = QString("Login failed with status code: %1").arg(m_lastStatusCode);
        qWarning() << errorMsg;
        emit loginFailed(errorMsg);
        reply->deleteLater();
        return;
    }

    // Parse JSON response
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (!jsonDoc.isObject()) {
        emit loginFailed("Invalid JSON response");
        reply->deleteLater();
        return;
    }

    parseLoginResponse(jsonDoc);
    reply->deleteLater();
}

void NetworkManager::parseLoginResponse(const QJsonDocument& jsonDoc)
{
    QJsonObject root = jsonDoc.object();

    QString token = root.value("token").toString();
    if (token.isEmpty()) {
        emit loginFailed("Token missing in response");
        return;
    }
    m_authToken = token;

    m_userId = root.value("id").toInt();

    emit userIdChanged();

    // Store full user data
    QJsonObject userData = root;
    m_lastUserData = userData;
    qDebug() << "Login successful. Token stored:" << m_authToken << "";
    // Emit success signal
    emit loginDone();
}

void NetworkManager::sendDraftToServer(const QJsonObject& payload)
{
    QUrl url("http://127.0.0.1:8080/api/v1/default/draft-entity");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader(
            "Authorization",
            QByteArray("Bearer ") + m_authToken.toUtf8()
            );
    }

    QByteArray data = QJsonDocument(payload).toJson();

    qDebug() << "Sending payload:\n"
             << QJsonDocument(payload).toJson(QJsonDocument::Indented);

    QNetworkReply* reply = m_manager->post(request, data);

    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Error sending draft:"
                       << reply->errorString();
            qWarning() << "Server response:"
                       << reply->readAll();
        } else {
            qDebug() << "Draft sent successfully:"
                     << reply->readAll();
        }
        reply->deleteLater();
    });
}

void NetworkManager::requestChangeLogSync()
{
    ProjectController projectController;
    int lastChangeLogId= projectController.getLastChangeLogId();

    QString urlString = "http://127.0.0.1:8080/api/v1/default/change-log";

    if (lastChangeLogId > 0) {
        urlString += "/since/" + QString::number(lastChangeLogId);
    }

    QUrl url(urlString);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader(
            "Authorization",
            QByteArray("Bearer ") + m_authToken.toUtf8()
            );
    }

    QNetworkReply* reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onChangeLogSyncFinished(reply);
    });
}

void NetworkManager::onChangeLogSyncFinished(QNetworkReply* reply)
{
    QByteArray rawData = reply->readAll();
    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Changelog error:" << reply->errorString();
        reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(rawData);

    if (doc.isArray()) {
        QJsonArray changelogs = doc.array();

        if (!changelogs.isEmpty()) {
            QJsonObject lastItem = changelogs.last().toObject();

            if (lastItem.contains("id")) {

                int lastChangeLogId = lastItem["id"].toInt();
                ProjectController projectController;
                projectController.updateLastChangeLogId(lastChangeLogId);
                qDebug() << "lastChangeLogId Updated" ;
            }
        }

        emit changeLogSyncReceived(changelogs);
    }

    reply->deleteLater();
}

