#include "repositories/work_billing_line_repository.h"
#include <QSqlQuery>
#include <QVariant>

WorkBillingLineRepository::WorkBillingLineRepository(QObject* parent)
    : QObject(parent)
{
}

std::unique_ptr<WorkBillingLine> WorkBillingLineRepository::findById(int id)
{
    return nullptr;
}

std::vector<std::unique_ptr<WorkBillingLine>> WorkBillingLineRepository::findAll()
{
    return {};
}

std::vector<WorkBillingLine*> WorkBillingLineRepository::findAllQML()
{
    std::vector<WorkBillingLine*> workBillingLine;
    QSqlQuery query(dbManager->getDatabase());
    if (query.exec("SELECT * FROM WorkBillingLine"))
    {
        while (query.next())
        {
            workBillingLine.push_back(mapFromQueryQML(query, this));
        }
    }
    return workBillingLine;
}

bool WorkBillingLineRepository::save(const WorkBillingLine& entity)
{
    return false;
}

bool WorkBillingLineRepository::saveQML(WorkBillingLine* entity)
{
    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);
    return query.exec();
}

bool WorkBillingLineRepository::update(const WorkBillingLine& entity)
{
    return false;
}

bool WorkBillingLineRepository::deleteById(int id)
{
    return false;
}

std::vector<std::unique_ptr<WorkBillingLine>> WorkBillingLineRepository::findByApprovalStatus(bool status)
{
    return {};
}

QString WorkBillingLineRepository::getTableName() const
{
    return "WorkBillingLine";
}

std::unique_ptr<WorkBillingLine> WorkBillingLineRepository::mapFromQuery(const QSqlQuery& query) const
{
    return nullptr;
}

WorkBillingLine* WorkBillingLineRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const
{
    auto workBillingLine = new WorkBillingLine(parent);
    workBillingLine->setId(query.value("id").toInt());
    workBillingLine->setGlobalId(query.value("global_id").toString());
    workBillingLine->setApprovalStatus(query.value("approval_status").toBool());
    workBillingLine->setWorkOrderId(query.value("workOrderId").toInt());
    workBillingLine->setWorkOrderLineId(query.value("work_order_line_id").toInt());

    return workBillingLine;
}

void WorkBillingLineRepository::bindEntityToQuery(QSqlQuery& query, const WorkBillingLine& entity) const
{
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getWorkOrderId());
    query.addBindValue(entity.getWorkOrderLineId());
}

QString WorkBillingLineRepository::getInsertQuery() const
{
    return "INSERT INTO WorkBillingLine (global_id, approval_status, work_order_id, work_billing_line) "
           "VALUES (?, ?, ?, ?)";
}

QString WorkBillingLineRepository::getUpdateQuery() const
{
    return "UPDATE WorkBillingLine SET global_id = ?, approval_status = ?, work_order_id = ?, "
           "work_billing_line = ? WHERE id = ?";
}
