#ifndef WORK_ORDER_LINE_CONTROLLER_H
#define WORK_ORDER_LINE_CONTROLLER_H

#include <QObject>
#include <QVariantList>
#include "repositories/work_order_line_repository.h"
#include "repositories/draft_entity_repository.h"
#include "repositories/vendor_repository.h"
#include "repositories/task_repository.h"

class WorkOrderLineController : public QObject
{
    Q_OBJECT
public:
    explicit WorkOrderLineController(QObject *parent = nullptr);

    Q_INVOKABLE void create(const int &vendorId,const QString &description,const QVariantList &workOrderLineData) const;

    Q_INVOKABLE void update(int id, const int &vendorId,const QString &description,const QVariantList &workOrderLineData) const;

    Q_INVOKABLE std::vector<WorkOrderLine*> getWorkOrderLineList(bool isApproved) const;

private:
    WorkOrderLineRepository *m_workOrderLineRepository;
    DraftEntityRepository *m_draftEntityRepository;
    VendorRepository *m_vendorRepository;
    TaskRepository *m_taskRepository;

    QString getVendorNameById(const int &vendorId) const;
    QString getTaskDescriptionById(const int &taskId) const;
};

#endif // WORK_ORDER_LINE_CONTROLLER_H
