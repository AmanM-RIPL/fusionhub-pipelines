#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <QObject>
#include "repositories/project_repository.h"
#include "common/ifcdetail.h"

class ProjectController : public QObject
{
    Q_OBJECT
public:
    explicit ProjectController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &projectName, const QString &customerName,
                            const QString &contactName, const QString &phoneNumber,
                            const QString &emailId, const QString &totalDollarValue,
                            const QString &description) const;

    Q_INVOKABLE QString getProjectList(bool isBlocked=false) const;

    Q_INVOKABLE void openDatabase(const QString &projectName, int projectId)const;

    Q_INVOKABLE int getLastSyncedOn()const;
    Q_INVOKABLE void updateLastSyncedOn(int lastSyncedOn)const;



private:
   ProjectRepository* m_projectRepository;


signals:


};

#endif // PROJECT_CONTROLLER_H
