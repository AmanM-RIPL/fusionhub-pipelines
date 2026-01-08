#include "repositories/task_repository.h"
TaskRepository::TaskRepository(QObject* parent) : QObject(parent) {}

std::unique_ptr<Task> TaskRepository::findById(int id) { return nullptr; }
std::vector<std::unique_ptr<Task>> TaskRepository::findAll() { return {}; }
std::vector<Task*> TaskRepository::findAllQML(const QString &status) {
    std::vector<Task*> tasks;
    QSqlQuery query(dbManager->getDatabase());

    if(status == nullptr)
    {
        if (query.exec("SELECT * FROM Task")) {
            while (query.next()) {
                tasks.push_back(mapFromQueryQML(query, this));
            }
        }
    }
    else{
        query.prepare("SELECT * FROM Task WHERE status = ?");
        query.addBindValue(status);

        if (query.exec()) {
            while (query.next()) {
                tasks.push_back(mapFromQueryQML(query, this));
            }
        }
    }

    return tasks;
}

bool TaskRepository::save(const Task& entity) { return false; }
bool TaskRepository::saveQML(Task* entity) {

    QSqlQuery query(dbManager->getDatabase());
    query.prepare(getInsertQuery());
    bindEntityToQuery(query, *entity);

    return query.exec();
}
bool TaskRepository::update(const Task& entity) { return false; }
bool TaskRepository::deleteById(int id) { return false; }
std::vector<std::unique_ptr<Task>> TaskRepository::findByApprovalStatus(bool status) { return {}; }
QString TaskRepository::getTableName() const { return "Task"; }
std::unique_ptr<Task> TaskRepository::mapFromQuery(const QSqlQuery& query) const { return nullptr; }

Task* TaskRepository::mapFromQueryQML(const QSqlQuery& query, QObject* parent) const {
    auto task = new Task(parent);
    task->setId(query.value("id").toInt());
    task->setGlobalId(query.value("global_id").toString());
    task->setApprovalStatus(query.value("approval_status").toBool());
    task->setTaskName(query.value("task_name").toString());
    task->setDescription(query.value("description").toString());
    task->setBimElement(query.value("bim_element").toString());
    task->setStartDate(query.value("start_date").toString());
    task->setEndDate(query.value("end_date").toString());
    task->setParentId(query.value("pid").toInt());
    task->setTaskStatus(query.value("status").toString());

    return task;
}
void TaskRepository::bindEntityToQuery(QSqlQuery& query, const Task& entity) const {
    query.addBindValue(entity.getGlobalId());
    query.addBindValue(entity.getApprovalStatus());
    query.addBindValue(entity.getTaskName());
    query.addBindValue(entity.getDescription());
    query.addBindValue(entity.getBimElement());
    query.addBindValue(entity.getStartDate());
    query.addBindValue(entity.getEndDate());
    query.addBindValue(entity.getParentId());
    query.addBindValue(entity.getTaskStatus());
}
QString TaskRepository::getInsertQuery() const {
    return "INSERT INTO Task (global_id, approval_status, task_name, description, bim_element, start_date, end_date, pid, status) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)";
}
QString TaskRepository::getUpdateQuery() const {
    return "UPDATE Task SET global_id = ?, approval_status = ?, task_name = ?, "
           "description = ?, bim_element = ?, start_date = ?, end_date = ?, pid = ?, status = ?";
}
