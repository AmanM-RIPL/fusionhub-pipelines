#include "models/user.h"

User::User(int id, const QString& globalId, bool approvalStatus, 
           const QString& userId, const QString& username)
    : id(id), globalId(globalId), approvalStatus(approvalStatus),
      userId(userId), username(username)
{
}
