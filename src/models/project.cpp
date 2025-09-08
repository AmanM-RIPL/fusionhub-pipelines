#include "project.h"
#include<QMessageBox>

Project::Project(int id, const QString& globalId, bool approvalStatus,
           const QString& projectName, const QString& customerName, const QString& contactName, const QString& phoneNumber,
                 const QString& emailId, const QString& totalDollarValue, const QString& description, bool isBlocked )
    : id(id), globalId(globalId), approvalStatus(approvalStatus), projectName(projectName),
    customerName(customerName), contactName(contactName), phoneNumber(phoneNumber), emailId(emailId),
    totalDollarValue(totalDollarValue), description(description), isBlocked(isBlocked)

{

}

