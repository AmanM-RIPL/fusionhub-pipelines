#ifndef SCHEDULE_SETUP_CONTROLLER_H
#define SCHEDULE_SETUP_CONTROLLER_H

#include <QObject>
#include <QList>
#include "repositories/schedule_setup_repository.h"
#include "repositories/draft_entity_repository.h"


class ScheduleSetupController : public QObject
{
    Q_OBJECT
public:
    explicit ScheduleSetupController(QObject *parent = nullptr);

    //Q_INVOKABLE void create(const QString &scheduleName, const QString &description, const QString &costParameter,
    //                        const QString &resourceParameter) const;
    Q_INVOKABLE void create(const QString &scheduleName, const QString &description, const QVariant &costParameter,
                            const QVariant &resourceParameter) const;
    Q_INVOKABLE void approvedCreate(const QString &scheduleSetupName,const QString &description,const QString &costParameter,const QString &resourceParameter) const;

    Q_INVOKABLE std::vector<ScheduleSetup*> getSetupList(bool isApproved = false) const;

signals:

private:
    ScheduleSetupRepository* m_scheduleSetupRepository;
    DraftEntityRepository* m_draftEntityRepository;

    QJsonDocument CreateJson(const QVariant &param) const;
};

#endif // SCHEDULE_SETUP_CONTROLLER_H
