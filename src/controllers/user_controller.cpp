#include "user_controller.h"
#include "common/repository_locator.h"

UserController::UserController(QObject *parent)
    :
    QObject{parent},
    m_userRepository(RepositoryLocator::instance().userRepository())
{}

bool UserController::login(const QString &username, const QString &password) const
{
    qInfo() << username << " " << password;
    return true; // implement NetworkManager later
}


void UserController::create(const QString& user_fullname,
                        const QString& user_name,
                        const QString& mobile1,
                        const QString& mobile2,
                        const QString& email1,
                        const QString& email2,
                        const QString& jobTitle,
                        const QString& startDate,
                        const QString& endDate,
                        const QString& monthlyDeskCostValue) const
{
    User user;

    user.setId(123);
    user.setGlobalId("123");
    user.setApprovalStatus(true);
    user.setUserId("010d");

    user.setUserFullName(user_fullname);
    user.setUserName(user_name);
    user.setUserMobile1(mobile1);
    user.setUserMobile2(mobile2);
    user.setUserEmail1(email1);
    user.setUserEmail2(email2);
    user.setUserJobTitle(jobTitle);
    user.setUserStartDate(startDate);
    user.setUserEndDate(endDate);
    user.setUserMonthlyDeskCostValue(monthlyDeskCostValue);

    if(m_userRepository->saveQML(&user))
    {
        qDebug()<<"Data Saved";
    }
    else
    {
        qDebug()<<"Data not Saved";
    }
}

std::vector<User*> UserController::getUserList() const
{
    return m_userRepository->findAllQML();
}

