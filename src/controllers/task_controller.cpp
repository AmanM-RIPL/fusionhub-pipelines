#include "task_controller.h"
#include "common/repository_locator.h"
#include <QDir>

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

    if(isApproved){
       // return m_taskRepository->findAllQML();
      std::vector<Task*> tasks = m_taskRepository->findAllQML();
       for(int i = 0; i < tasks.size(); i++)
       {
           //QString startData = tasks[i]->getStartDate();
           //QString endDate = tasks[i]->getEndDate();

           QDate startDate = QDate::fromString(tasks[i]->getStartDate(), "dd/MM/yyyy");
           QDate endDate = QDate::fromString(tasks[i]->getEndDate(), "dd/MM/yyyy");

           if (startDate.isValid() && endDate.isValid()) {
               qint64 days = startDate.daysTo(endDate);
               tasks[i]->setDuration(days);

               tasks[i]->setYear(startDate.year());
               tasks[i]->setMonth(startDate.month());
               tasks[i]->setStartDay(startDate.day());
               tasks[i]->setEndDay(endDate.day());

               QString strDays = QString::number(days);
               int monthNo = startDate.month();
              /* switch(monthNo)
               {
               case 1:
                   tasks[i]->setJan(strDays);
               case 2:
                   tasks[i]->setFeb(strDays);
               case 3:
                   tasks[i]->setMar(strDays);
               case 4:
                   tasks[i]->setApr(strDays);
               case 5:
                   tasks[i]->setMay(strDays);
               case 6:
                   tasks[i]->setJun(strDays);
               case 7:
                   tasks[i]->setJul(strDays);
               case 8:
                   tasks[i]->setAug(strDays);
               case 9:
                   tasks[i]->setSep(strDays);
               case 10:
                   tasks[i]->setOct(strDays);
               case 11:
                   tasks[i]->setNov(strDays);
               case 12:
                   tasks[i]->setDec(strDays);
               }*/

               if(monthNo == 1)
               {
                 tasks[i]->setJan(strDays);
               }
               else
               {
                   tasks[i]->setJan(" ");
               }

               if(monthNo == 2)
               {
                   tasks[i]->setFeb(strDays);
               }
               else
               {
                   tasks[i]->setFeb(" ");
               }

               if(monthNo == 3)
               {
                   tasks[i]->setMar(strDays);
               }
               else
               {
                   tasks[i]->setMar(" ");
               }

               if(monthNo == 4)
               {
                   tasks[i]->setApr(strDays);
               }
               else
               {
                   tasks[i]->setApr(" ");
               }

               if(monthNo == 5)
               {
                   tasks[i]->setMay(strDays);
               }
               else
               {
                   tasks[i]->setMay(" ");
               }

               if(monthNo == 6)
               {
                   tasks[i]->setJun(strDays);
               }
               else
               {
                   tasks[i]->setJun(" ");
               }

               if(monthNo ==7)
               {
                   tasks[i]->setJul(strDays);
               }
               else
               {
                   tasks[i]->setJul(" ");
               }

               if(monthNo == 8)
               {
                   tasks[i]->setAug(strDays);
               }
               else
               {
                   tasks[i]->setAug(" ");
               }

               if(monthNo == 9)
               {
                   tasks[i]->setSep(strDays);
               }
               else
               {
                   tasks[i]->setSep(" ");
               }

               if(monthNo == 10)
               {
                   tasks[i]->setOct(strDays);
               }
               else
               {
                   tasks[i]->setOct(" ");
               }

               if(monthNo == 11)
               {
                   tasks[i]->setNov(strDays);
               }
               else
               {
                   tasks[i]->setNov(" ");
               }

               if(monthNo == 12)
               {
                   tasks[i]->setDec(strDays);
               }
               else
               {
                   tasks[i]->setDec(" ");
               }


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
                //task->setId(i + 1);
                task->setId(jsonObj["id"].toInt());
                task->setGlobalId("123");
                task->setApprovalStatus(true);
                task->setTaskName(jsonObj["task_name"].toString());
                task->setDescription(jsonObj["description"].toString());
                task->setBimElement(jsonObj["bim_element"].toString());
                task->setStartDate(jsonObj["start_date"].toString());
                task->setEndDate(jsonObj["end_date"].toString());
                task->setParentId(jsonObj["pid"].toInt());

                QDate startDate = QDate::fromString(task->getStartDate(), "dd/MM/yyyy");
                QDate endDate = QDate::fromString(task->getEndDate(), "dd/MM/yyyy");

                if (startDate.isValid() && endDate.isValid()) {
                    qint64 days = startDate.daysTo(endDate);
                    task->setDuration(days);

                    task->setYear(startDate.year());
                    task->setMonth(startDate.month());
                    task->setStartDay(startDate.day());
                    task->setEndDay(endDate.day());

                    QString strDays = QString::number(days);
                    int monthNo =startDate.month();

                    if(monthNo == 1)
                    {
                        task->setJan(strDays);
                    }
                    else
                    {
                        task->setJan(" ");
                    }

                    if(monthNo == 2)
                    {
                        task->setFeb(strDays);
                    }
                    else
                    {
                        task->setFeb(" ");
                    }

                    if(monthNo == 3)
                    {
                        task->setMar(strDays);
                    }
                    else
                    {
                        task->setMar(" ");
                    }

                    if(monthNo == 4)
                    {
                        task->setApr(strDays);
                    }
                    else
                    {
                        task->setApr(" ");
                    }

                    if(monthNo == 5)
                    {
                        task->setMay(strDays);
                    }
                    else
                    {
                        task->setMay(" ");
                    }

                    if(monthNo == 6)
                    {
                        task->setJun(strDays);
                    }
                    else
                    {
                        task->setJun(" ");
                    }

                    if(monthNo ==7)
                    {
                        task->setJul(strDays);
                    }
                    else
                    {
                        task->setJul(" ");
                    }

                    if(monthNo == 8)
                    {
                        task->setAug(strDays);
                    }
                    else
                    {
                        task->setAug(" ");
                    }

                    if(monthNo == 9)
                    {
                        task->setSep(strDays);
                    }
                    else
                    {
                        task->setSep(" ");
                    }

                    if(monthNo == 10)
                    {
                        task->setOct(strDays);
                    }
                    else
                    {
                        task->setOct(" ");
                    }

                    if(monthNo == 11)
                    {
                        task->setNov(strDays);
                    }
                    else
                    {
                        task->setNov(" ");
                    }

                    if(monthNo == 12)
                    {
                        task->setDec(strDays);
                    }
                    else
                    {
                        task->setDec(" ");
                    }


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
