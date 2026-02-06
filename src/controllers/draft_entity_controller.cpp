#include "draft_entity_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

DraftEntityController::DraftEntityController(QObject *parent)
    : QObject(parent),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}
    void DraftEntityController::approve(int id) const
    {
        DraftEntity draftEntity;
        draftEntity.setId(id);
        draftEntity.setTenant(gTenantId);
        // draftEntity.setProject(gProjectId);
        draftEntity.setNextApprovingUser(0);
        QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
        QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

        QJsonObject jsonObjectChangeHistory;
        jsonObjectChangeHistory["user"] = gUser->getId();
        jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
        jsonObjectChangeHistory["changeType"] = "Approve";
        jsonObjectChangeHistory["description"] = "Approved By User";
        jsonObjectChangeHistory["approvalHistory"] = "Approved";

        QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
        QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

        QDate approvedOn = QDate::currentDate();

        draftEntity.setChangeHistory(changeHistory);
        draftEntity.setCreatedOn(approvedOn);
        draftEntity.setApprovalStatus("Approved");
        // draftEntity.setApprovedByUser(gUser->getId());

        m_draftEntityRepository->approveQML(&draftEntity);
    }

    void DraftEntityController::cancel(int id, const QString &cancellationReason) const
    {
        DraftEntity draftEntity;
        draftEntity.setId(id);
        draftEntity.setTenant(gTenantId);
        draftEntity.setNextApprovingUser(0);

        // Create change history for cancellation
        QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
        QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

        QJsonObject jsonObjectChangeHistory;
        jsonObjectChangeHistory["user"] = gUser->getId();
        jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
        jsonObjectChangeHistory["changeType"] = "cancel";
        jsonObjectChangeHistory["approvalHistory"] = cancellationReason;

        QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
        QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

        QDate cancelledOn = QDate::currentDate();

        draftEntity.setChangeHistory(changeHistory);
        draftEntity.setCreatedOn(cancelledOn);
        draftEntity.setApprovalStatus("cancelled");  // Update status to cancelled
        // draftEntity.setCancelledByUser(gUser->getId());

        m_draftEntityRepository->cancelQML(&draftEntity);
    }
