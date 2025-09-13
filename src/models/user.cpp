#include "models/user.h"

User::User(int id,
     const QString& globalId,
     bool approvalStatus,
     const QString& user_id,
     const QString& user_fullname,
     const QString& user_name,
     const QString& mobile1,
     const QString& mobile2,
     const QString& email1,
     const QString& email2,
     const QString& jobTitle,
     const QString& startDate,
     const QString& endDate,
     const QString& monthlyDeskCostValue,
           QObject* parent)
    :id(id),
    globalId(globalId),
    approvalStatus(approvalStatus),
    user_id(user_id),
    user_fullname(user_fullname),
    user_name(user_name),
    mobile1(mobile1),
    mobile2(mobile2),
    email1(email1),
    email2(email2),
    jobTitle(jobTitle),
    startDate(startDate),
    endDate(endDate),
    monthlyDeskCostValue(monthlyDeskCostValue),
    QObject(parent)
{

}





