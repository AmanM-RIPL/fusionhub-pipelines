#include "task_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QMap>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

TaskController::TaskController(QObject *parent)
    : QObject{parent},
    m_taskRepository(RepositoryLocator::instance().taskRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void TaskController::create(const QString &name, const QString &description, const QString &bimElement, const QString &startDate , const QString &endDate, const int pid) const
{
    Task task;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    task.setId(id_in_milliseconds);
    task.setGlobalId("123");
    task.setApprovalStatus(true);
    task.setTaskName(name);
    task.setDescription(description);
    task.setBimElement(bimElement);
    task.setStartDate(startDate);
    task.setEndDate(endDate);
    task.setParentId(pid);

    m_taskRepository->saveQML(&task);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["task_name"] = name;
    jsonObject["description"] = description;
    jsonObject["bim_element"] = bimElement;
    jsonObject["start_date"] = startDate;
    jsonObject["end_date"] = endDate;
    jsonObject["pid"] = pid;    

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "Task::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    //jsonObjectChangeHistory["user"] = gUser->getUserId();
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["description"] = "Cretaed By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("Task");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<Task*> TaskController::getTaskList(bool isApproved) const
{    
    qDebug()<<"IsApproved: "<< isApproved;

    QMap<int,QString> monthMap ={{1, "jan"}, {2, "feb"}, {3, "mar"},
                                 {4, "apr"}, {5, "may"}, {6,"jun"},
                                 {4, "jul"}, {5, "aug"}, {6,"sep"},
                                 {4, "oct"}, {5, "nov"}, {6,"dec"},
                                };

    if(isApproved){
       // return m_taskRepository->findAllQML();
      std::vector<Task*> tasks = m_taskRepository->findAllQML();
       for(int i = 0; i < tasks.size(); i++)
       {
           //QString startData = tasks[i]->getStartDate();
           //QString endDate = tasks[i]->getEndDate();

           QString dateFormat = "dd/MM/yyyy";
           //QString dateFormat = "YYYY-MM-DD";

           QDate startDate = QDate::fromString(tasks[i]->getStartDate(), dateFormat);
           QDate endDate = QDate::fromString(tasks[i]->getEndDate(), dateFormat);


           if (startDate.isValid() && endDate.isValid()) {

               tasks[i]->setStartYear(startDate.year());
               tasks[i]->setEndYear(endDate.year());

               tasks[i]->setStartMonth(startDate.month());
               tasks[i]->setEndMonth(endDate.month());

               qint64 days = startDate.daysTo(endDate);
               tasks[i]->setDuration(days);

               tasks[i]->setYear(startDate.year());
               tasks[i]->setMonth(startDate.month());
               tasks[i]->setStartDay(startDate.day());
               tasks[i]->setEndDay(endDate.day());               

               QString strDays = QString::number(days);
               int monthNo = startDate.month();

               tasks[i]->setMonth_Year(monthMap[monthNo] + "_" +  QString::number(startDate.year()));

               setDaysInMonth(tasks[i], strDays, monthNo, tasks[i]->getMonth_Year());
           } else {
                qDebug() << "One or both dates are invalid.";
                tasks[i]->setDuration(0);
           }
       }
       return tasks;
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("Task");
        std::vector<Task*> tasks;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto task = new Task();
                QJsonObject jsonObj = jsonDoc.object();
                task->setId(i + 1);
               // task->setId(jsonObj["id"].toInt());
                task->setGlobalId("123");
                task->setApprovalStatus(true);
                task->setTaskName(jsonObj["task_name"].toString());
                task->setDescription(jsonObj["description"].toString());
                task->setBimElement(jsonObj["bim_element"].toString());
                task->setStartDate(jsonObj["start_date"].toString());
                task->setEndDate(jsonObj["end_date"].toString());
                task->setParentId(jsonObj["pid"].toInt());

                QString dateFormat = "dd/MM/yyyy";
               // QString dateFormat = "YYYY-MM-DD";

                QDate startDate = QDate::fromString(task->getStartDate(), dateFormat);
                QDate endDate = QDate::fromString(task->getEndDate(), dateFormat);


                if (startDate.isValid() && endDate.isValid()) {

                    task->setStartYear(startDate.year());
                    task->setEndYear(endDate.year());

                    task->setStartMonth(startDate.month());
                    task->setEndMonth(endDate.month());

                    qint64 days = startDate.daysTo(endDate);
                    task->setDuration(days);

                    task->setYear(startDate.year());
                    task->setMonth(startDate.month());
                    task->setStartDay(startDate.day());
                    task->setEndDay(endDate.day());

                    QString strDays = QString::number(days);
                    int monthNo = startDate.month();

                    task->setMonth_Year(monthMap[monthNo] + "_" + QString::number(startDate.year()));

                    setDaysInMonth(task, strDays, monthNo, task->getMonth_Year());

                } else {
                    qDebug() << "One or both dates are invalid.";
                    task->setDuration(0);
                }

                tasks.push_back(task);
            }
        }
        return tasks;
    }
}

void TaskController::setDaysInMonth(Task* task, const QString& strDays, const int monthNo, const QString& startDate ) const
{

    if(monthNo == 1)
    {
        //task->setJan(strDays + "_" + startDate);
        task->setJan(startDate);
    }
    else
    {
        task->setJan(" ");        
    }

    if(monthNo == 2)
    {
        //task->setFeb(strDays + "_" + startDate);
        task->setFeb(startDate);
    }
    else
    {
        task->setFeb(" ");        
    }

    if(monthNo == 3)
    {
        //task->setMar(strDays + "_" + startDate);
        task->setMar(startDate);

    }
    else
    {
        task->setMar(" ");        
    }

    if(monthNo == 4)
    {
        //task->setApr(strDays + "_" + startDate);
         task->setApr(startDate);

    }
    else
    {
        task->setApr(" ");        
    }

    if(monthNo == 5)
    {
        //task->setMay(strDays + "_" + startDate);
        task->setMay(startDate);
    }
    else
    {
        task->setMay(" ");       
    }

    if(monthNo == 6)
    {
        //task->setJun(strDays + "_" + startDate);
        task->setJun(startDate);
    }
    else
    {
        task->setJun(" ");        
    }

    if(monthNo == 7)
    {
        //task->setJul(strDays + "_" + startDate);
        task->setJul(startDate);
    }
    else
    {
        task->setJul(" ");        
    }

    if(monthNo == 8)
    {
       // task->setAug(strDays + "_" + startDate);
        task->setAug(startDate);
    }
    else
    {
        task->setAug(" ");        
    }

    if(monthNo == 9)
    {
        //task->setSep(strDays + "_" + startDate);
        task->setSep(startDate);
    }
    else
    {
        task->setSep(" ");        
    }

    if(monthNo == 10)
    {
        //task->setOct(strDays + "_" + startDate);
        task->setOct(startDate);
    }
    else
    {
        task->setOct(" ");        
    }

    if(monthNo == 11)
    {
        //task->setNov(strDays + "_" + startDate);
        task->setNov(startDate);
    }
    else
    {
        task->setNov(" ");        
    }

    if(monthNo == 12)
    {
        //task->setDec(strDays + "_" + startDate);
        task->setDec(startDate);

    }
    else
    {
        task->setDec(" ");        
    }
}
