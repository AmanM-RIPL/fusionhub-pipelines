#ifndef USER_H
#define USER_H

#include <QString>
#include <QMetaType>

class User
{
public:
    User() = default;
    User(int id, const QString& globalId, bool approvalStatus, 
         const QString& userId, const QString& username);
    
    int getId() const { return id; }
    QString getGlobalId() const { return globalId; }
    bool getApprovalStatus() const { return approvalStatus; }
    QString getUserId() const { return userId; }
    QString getUsername() const { return username; }
    
    void setId(int id) { this->id = id; }
    void setGlobalId(const QString& globalId) { this->globalId = globalId; }
    void setApprovalStatus(bool status) { this->approvalStatus = status; }
    void setUserId(const QString& userId) { this->userId = userId; }
    void setUsername(const QString& username) { this->username = username; }

private:
    int id = 0;
    QString globalId;
    bool approvalStatus = true;
    QString userId;
    QString username;
};

Q_DECLARE_METATYPE(User)

#endif
