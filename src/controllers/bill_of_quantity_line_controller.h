#ifndef BILL_OF_QUANTITY_LINE_CONTROLLER_H
#define BILL_OF_QUANTITY_LINE_CONTROLLER_H

#include <QObject>
#include "repositories/bill_of_quantity_line_repository.h"
#include "repositories/bill_of_quantity_repository.h"
#include "repositories/task_repository.h"
#include "repositories/draft_entity_repository.h"

class BillOfQuantityLineController: public QObject
{
    Q_OBJECT
public:
    explicit BillOfQuantityLineController(QObject *parent = nullptr);
    Q_INVOKABLE void create(const QString &name, const int &BillOfQuantityId , const int &TaskId ) const;
    Q_INVOKABLE std::vector<BillOfQuantityLine*> getBillOfQuantityLineList(bool isApproved = false) const;

    Q_INVOKABLE std::vector<BillOfQuantity*> getBOQList() const;
     Q_INVOKABLE std::vector<Task*> getTaskList() const;

private:
    BillOfQuantityLineRepository* m_billOfQuantityLineRepository;
    BillOfQuantityRepository* m_billOfQuantityRepository;
    TaskRepository* m_taskRepository;
    DraftEntityRepository* m_draftEntityRepository;
};

#endif // BILL_OF_QUANTITY_LINE_CONTROLLER_H

