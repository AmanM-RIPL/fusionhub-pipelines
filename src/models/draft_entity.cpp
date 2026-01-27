#include "draft_entity.h"


DraftEntity::DraftEntity(int id, int tenant, const QDate& createdOn, int project,
                         const QString& entity, int createdByUser, int nextApprovingUser, const QString& approvalStatus, const QString& globalId,
                         const QString& entitySchema, int associatedApprovedEntity, const QString& changeHistory, QObject* parent)
    : QObject(parent), id(id), tenant(tenant), createdOn(createdOn), project(project), entity(entity),
    createdByUser(createdByUser),  nextApprovingUser(nextApprovingUser), approvalStatus(approvalStatus), globalId(globalId), entitySchema(entitySchema),
    associatedApprovedEntity(associatedApprovedEntity), changeHistory(changeHistory)
{
}
