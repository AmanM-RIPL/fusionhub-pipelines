#ifndef WORK_BILLING_LINE_CONTROLLER_H
#define WORK_BILLING_LINE_CONTROLLER_H

#include <QObject>
#include <QList>
#include "repositories/work_billing_line_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/task_repository.h"


class WorkBillingLineController : public QObject
{
    Q_OBJECT
public:
    explicit WorkBillingLineController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const int &workOrderId, const QString &description, const QVariantList &workBillingLine) const;

    Q_INVOKABLE void update(int id, const int &workOrderId,const QString &description,const QVariantList &workBillingLineData) const;

    Q_INVOKABLE std::vector<WorkBillingLine*> getWorkBillingLineList(bool isApproved = false) const;
    Q_INVOKABLE  std::vector<Task*> getBilledTaskList(bool isApproved) const;

signals:

private:
    WorkBillingLineRepository* m_workBillingLineRepository;
    DraftEntityRepository* m_draftEntityRepository;

    QString CreateJson(const QVariant &param) const;
};

#endif // WORK_BILLING_LINE_CONTROLLER_H
