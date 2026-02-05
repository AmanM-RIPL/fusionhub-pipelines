#ifndef WORK_BILLING_CONTROLLER_H
#define WORK_BILLING_CONTROLLER_H

#include <QObject>
#include <QList>
#include "repositories/work_billing_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/task_repository.h"


class WorkBillingController : public QObject
{
    Q_OBJECT
public:
    explicit WorkBillingController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const QString &WorkBillingName, const int WorkOrderId ) const;
    Q_INVOKABLE void approvedCreate(const QString &WorkBillingName, const int WorkOrderId) const;


    Q_INVOKABLE std::vector<WorkBilling*> getWorkBillingList(bool isApproved = false) const;

signals:

private:
    WorkBillingRepository* m_workBillingRepository;
    DraftEntityRepository* m_draftEntityRepository;

    QString CreateJson(const QVariant &param) const;
};

#endif // WORK_BILLING_CONTROLLER_H

