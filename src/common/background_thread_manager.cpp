#include "background_thread_manager.h"
#include "network/network_manager.h"
#include <QDebug>
#include "common/repository_locator.h"

BackgroundThreadManager* BackgroundThreadManager::m_instance = nullptr;

BackgroundThreadManager::BackgroundThreadManager(QObject *parent)
    : QObject(parent)
{
}

BackgroundThreadManager* BackgroundThreadManager::instance()
{
    if (!m_instance)
    {
        m_instance = new BackgroundThreadManager();
    }
    return m_instance;
}

void BackgroundThreadManager::runBackgroundTaskForDraftDataSync()
{

    auto* repo = RepositoryLocator::instance().draftEntityRepository();

    auto drafts = repo->findAllApprovedQML();

    if (drafts.empty()) {
        qDebug() << "No draft data found.";
        return;
    }

    auto* network = NetworkManager::getInstance();

    for (auto* draft : drafts)
    {
        if (!draft) continue;

        QJsonObject payload;

        payload["project"] = draft->getProject();
        payload["entity"]  = draft->getEntity();

        payload["createdOn"] = draft->getCreatedOn().toString(Qt::ISODate);

        payload["createdByUser"] = draft->getCreatedByUser();

        payload["nextApprovingUser"] = draft->getNextApprovingUser();

        payload["associatedApprovedEntity"] = draft->getAssociatedApprovedEntity();

        // -------- entitySchema ----------
        QJsonDocument schemaDoc = QJsonDocument::fromJson(draft->getEntitySchema().toUtf8() );

        payload["entitySchema"] = schemaDoc.object();

        // -------- changeHistory ----------
        QJsonDocument historyDoc = QJsonDocument::fromJson(draft->getChangeHistory().toUtf8());

        payload["changeHistory"] = historyDoc.object();

        // qDebug() << "Sending payload:\n"
        //          << QJsonDocument(payload)
        //                 .toJson(QJsonDocument::Indented);

        network->sendDraftToServer(payload);
    }

    if (repo->deleteAllApprovedQML()) {
        qDebug() << "Approved draft entities deleted after successful sync.";
    } else {
        qDebug() << "Failed to delete approved draft entities after sync.";
    }
}
