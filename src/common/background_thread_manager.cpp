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

void BackgroundThreadManager::runBackgroundTaskForChangeLogSync()
{
    auto* network = NetworkManager::getInstance();
    // Connect signal to process when data arrives
    connect(network, &NetworkManager::changeLogSyncReceived, this,
            &BackgroundThreadManager::onChangeLogSyncReceived, Qt::UniqueConnection);
    // Request the changelog (data will arrive in onChangeLogSyncReceived)
    network->requestChangeLogSync();
}


void BackgroundThreadManager::onChangeLogSyncReceived(const QJsonArray &arr)
{
    for (const auto &v : arr) {
        if (!v.isObject()) continue;

        QJsonObject obj = v.toObject();
        QString entity = obj["entity"].toString();
        QJsonObject changeHistory = obj["changeHistory"].toObject();
        QString changeType = changeHistory["changeType"].toString();

        QJsonObject schema = obj["entitySchema"].toObject();
        if (entity == "Material") {
            QString name = schema["material_name"].toString();
            QString category = schema["category"].toString();
            int unitOfMeasurementId = schema["unit_of_measurement_id"].toInt();

            auto* materialRepo = RepositoryLocator::instance().materialRepository();

            Material material;
            material.setGlobalId("123");
            material.setMaterialName(name);
            material.setCategory(category);
            material.setUnitOfMeasurementId(unitOfMeasurementId);

            materialRepo->saveQML(&material);

            qDebug() << " Material Created:" << name << category << unitOfMeasurementId;
        }
        // else if (entity == "WorkOrder") {
        //     QString workOrderNo = schema["workOrderNo"].toString();
        //     QString description = schema["description"].toString();

        //     auto* workOrderRepo = RepositoryLocator::instance().workOrderRepository();

        //     WorkOrder workOrder;
        //     workOrder.setGlobalId("123");
        //     workOrder.setWorkOrderNo(workOrderNo);
        //     workOrder.setDescription(description);
        //     workOrderRepo->saveQML(&workOrder);

        //     qDebug() << "WorkOrder Created:" << workOrderNo;
    }

}
