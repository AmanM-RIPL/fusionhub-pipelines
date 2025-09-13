#ifndef USER_H
#define USER_H

#include <QString>
#include <QMetaType>
#include <QObject>

class User:public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(bool approvalStatus READ getApprovalStatus WRITE setApprovalStatus /*NOTIFY approvalStatusChanged*/)
    Q_PROPERTY(QString globalId READ getGlobalId WRITE setGlobalId /*NOTIFY globalIdChanged*/)
    Q_PROPERTY(QString user_id READ getUserId WRITE setUserId /*NOTIFY userIdChanged*/)
    Q_PROPERTY(QString user_fullname READ getUserFullName WRITE setUserFullName /*NOTIFY userFullNameChanged*/)
    Q_PROPERTY(QString user_name READ getUserName WRITE setUserName /*NOTIFY userNameChanged*/)
    Q_PROPERTY(QString mobile1 READ getUserMobile1 WRITE setUserMobile1 /*NOTIFY userMobile1Changed*/)
    Q_PROPERTY(QString mobile2 READ getUserMobile2 WRITE setUserMobile2 /*NOTIFY userMobile2Changed*/)
    Q_PROPERTY(QString email1 READ getUserEmail1 WRITE setUserEmail1 /*NOTIFY userEmail1Changed*/)
    Q_PROPERTY(QString email2 READ getUserEmail2 WRITE setUserEmail2 /*NOTIFY userEmail2Changed*/)
    Q_PROPERTY(QString jobTitle READ getUserJobTitle WRITE setUserJobTitle /*NOTIFY userJobTitleChanged*/)
    Q_PROPERTY(QString startDate READ getUserStartDate WRITE setUserStartDate /*NOTIFY userStartDateChanged*/)
    Q_PROPERTY(QString endDate READ getUserEndDate WRITE setUserEndDate /*NOTIFY userEndDateChanged*/)
    Q_PROPERTY(QString monthlyDeskCostValue READ getUserMonthlyDeskCostValue WRITE setUserMonthlyDeskCostValue /*NOTIFY userMonthlyDeskCostValueChanged*/)

public:
    //User() = default;
    /*User(int id, const QString& globalId, bool approvalStatus,
         const QString& userId, const QString& username);*/

     explicit User(QObject* parent = nullptr): QObject(parent) {}

        User(int id,
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
             QObject* parent = nullptr);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getUserId() const { return user_id; }
    QString getUserFullName() const { return user_fullname; }
    QString getUserName() const { return user_name; }
    QString getUserMobile1() const { return mobile1; }
    QString getUserMobile2() const { return mobile2; }
    QString getUserEmail1() const { return email1; }
    QString getUserEmail2() const { return email2; }
    QString getUserJobTitle() const { return jobTitle; }
    QString getUserStartDate() const { return startDate; }
    QString getUserEndDate() const { return endDate; }
    QString getUserMonthlyDeskCostValue() const { return monthlyDeskCostValue; }

    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setUserId(const QString& user_id) { this->user_id = user_id; }
    void setUserFullName(const QString& user_fullname) { this->user_fullname = user_fullname; }
    void setUserName(const QString& user_name) { this->user_name = user_name ; }
    void setUserMobile1(const QString& mobile1) { this->mobile1 = mobile1; }
    void setUserMobile2(const QString& mobile2) { this->mobile2 = mobile2; }
    void setUserEmail1(const QString& email1) { this->email1 = email1; }
    void setUserEmail2(const QString& email2) { this->email2 = email2; }
    void setUserJobTitle(const QString& jobTitle) { this->jobTitle = jobTitle; }
    void setUserStartDate(const QString& startDate) { this->startDate = startDate; }
    void setUserEndDate(const QString& endDate) { this->endDate = endDate; }
    void setUserMonthlyDeskCostValue(const QString& monthlyDeskCostValue) { this->monthlyDeskCostValue = monthlyDeskCostValue; }

private:
    int id = 2;
    QString globalId= "123";
    bool approvalStatus = true;
    QString user_id = "ab03";
    QString user_fullname;
    QString user_name;
    QString mobile1;
    QString mobile2;
    QString email1;
    QString email2;
    QString jobTitle;
    QString startDate;
    QString endDate;
    QString monthlyDeskCostValue;
};

Q_DECLARE_METATYPE(User)

#endif
