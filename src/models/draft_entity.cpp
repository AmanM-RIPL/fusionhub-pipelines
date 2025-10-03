#include "draft_entity.h"


DraftEntity::DraftEntity(int id, int tenant, const QDate& createdOn, int project,
                         const QString& entity, const QString& createdByUser, int nextApprovingUser,
                         const QString& entitySchema, int associatedApprovedEntity, const QString& changeHistory, QObject* parent)
    : QObject(parent), id(id), tenant(tenant), createdOn(createdOn), project(project), entity(entity),
    createdByUser(createdByUser),  nextApprovingUser(nextApprovingUser), entitySchema(entitySchema),
    associatedApprovedEntity(associatedApprovedEntity), changeHistory(changeHistory)
{
}
