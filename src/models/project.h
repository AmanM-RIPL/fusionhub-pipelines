#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QMetaType>
#include <QObject>

class Project: public QObject
{
     Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus)
    Q_PROPERTY(QString globalId READ getGlobalId)


     Q_PROPERTY(QString projectName READ getProjectName  WRITE setProjectName NOTIFY ProjectNameChanged)
     Q_PROPERTY(QString customerName READ getCustomerName  WRITE setCustomerName NOTIFY CustomerNameChanged)
     Q_PROPERTY(QString contactName READ getContactName  WRITE setContactName NOTIFY ContactNameChanged)
     Q_PROPERTY(QString phoneNumber READ getPhoneNumber  WRITE setPhoneNumber NOTIFY PhoneNumberChanged)
     Q_PROPERTY(QString emailId READ getEmailId  WRITE setEmailId NOTIFY EmailIdChanged)
     Q_PROPERTY(QString totalDollarValue READ getTotalDollarValue  WRITE setTotalDollarValue NOTIFY TotalDollarValueChanged)
     Q_PROPERTY(QString description READ getDescription  WRITE setDescription NOTIFY DescriptionChanged)
     Q_PROPERTY(bool isBlocked READ getIsBlocked  WRITE setIsBlocked NOTIFY IsBlockedChanged)


public:
    //Project()= default;
    explicit Project(QObject* parent = nullptr): QObject(parent) {}

    Project(int id, const QString& globalId, bool approvalStatus,
         const QString& projectName, const QString& customerName, const QString& contactName, const QString& phoneNumber,
            const QString& emailId, const QString& totalDollarValue, const QString& description, bool isBlocked, QObject* parent = nullptr);

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

    signals:
    void ProjectNameChanged();
    void CustomerNameChanged();
    void ContactNameChanged();
    void PhoneNumberChanged();
    void EmailIdChanged();
    void TotalDollarValueChanged();
    void DescriptionChanged();
    void IsBlockedChanged();

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
Q_DECLARE_METATYPE(Project)

#endif // PROJECT_H
