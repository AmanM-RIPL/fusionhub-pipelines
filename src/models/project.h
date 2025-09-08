#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QMetaType>

class Project
{
public:
    Project()= default;

    Project(int id, const QString& globalId, bool approvalStatus,
         const QString& projectName, const QString& customerName, const QString& contactName, const QString& phoneNumber,
            const QString& emailId, const QString& totalDollarValue, const QString& description, bool isBlocked);

    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getProjectName() const { return projectName; }
    QString getCustomerName() const{return customerName;}
    QString getContactName() const{return contactName;}
    QString getPhoneNumber() const{return phoneNumber;}
    QString getEmailId() const{return emailId;}
    QString getTotalDollarValue() const{return totalDollarValue;}
    QString getDescription() const { return description; }
    bool getIsBlocked() const { return isBlocked; }


    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setProjectName(const QString& projectName) { this->projectName = projectName; }
    void setCustomerName(const QString& customerName) { this->customerName = customerName; }
    void setContactName(const QString& contactName) { this->contactName = contactName; }
    void setPhoneNumber(const QString& phoneNumber) { this->phoneNumber = phoneNumber; }
    void setEmailId(const QString& emailId) { this->emailId = emailId; }
    void setTotalDollarValue(const QString& totalDollarValue) { this->totalDollarValue = totalDollarValue; }
    void setDescription(const QString& description) { this->description = description; }
    void setIsBlocked(const bool isBlocked) { this->isBlocked = isBlocked; }

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString projectName;
    QString customerName;
    QString contactName;
    QString phoneNumber;
    QString emailId;
    QString totalDollarValue;
    QString description;
    bool isBlocked = false;
};

#endif // PROJECT_H
