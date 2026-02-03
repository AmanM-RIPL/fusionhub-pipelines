#include "project_budget_controller.h"
#include "budget_head_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

ProjectBudgetController::ProjectBudgetController(QObject *parent)
    : QObject{parent},
    m_projectBudgetRepository(RepositoryLocator::instance().projectBudgetRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void ProjectBudgetController::create(const QString &name, const int &budgetHeadId) const
{
    QJsonObject jsonObject;
    jsonObject["dollarValue"] = name;
    jsonObject["budgetHeadId"] = budgetHeadId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "ProjectBudget::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
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
    draftEntity.setEntity("ProjectBudget");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

void ProjectBudgetController::approvedCreate(const int &budgetHeadId, const QString &value) const
{
    ProjectBudget projectBudget;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    projectBudget.setId(id_in_milliseconds);
    projectBudget.setGlobalId("123");
    projectBudget.setApprovalStatus(true);
    projectBudget.setDollarValue(value);
    projectBudget.setBudgetHeadId(budgetHeadId);

    m_projectBudgetRepository->saveQML(&projectBudget);

}

void ProjectBudgetController::update(int id, const QString &name, const int &budgetHeadId) const
{
    QJsonObject jsonObject;
    jsonObject["dollarValue"] = name;
    jsonObject["budgetHeadId"] = budgetHeadId;
    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] = "update";
    jsonObjectChangeHistory["description"] = "Updated By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);

    QDate updatedOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(id);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(updatedOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("ProjectBudget");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<ProjectBudget*> ProjectBudgetController::getProjectBudgetList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_projectBudgetRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("ProjectBudget");
        std::vector<ProjectBudget*> projectBudgets;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            int draftId = draftEntitys[i]->getId();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto projectBudget = new ProjectBudget();
                QJsonObject jsonObj = jsonDoc.object();
                projectBudget->setId(draftId);
                projectBudget->setGlobalId("123");
                projectBudget->setApprovalStatus(true);

                projectBudget->setDollarValue(jsonObj["dollarValue"].toString());
                projectBudget->setBudgetHeadId(jsonObj["budgetHeadId"].toInt());

                projectBudgets.push_back(projectBudget);
            }
        }
        return projectBudgets;
    }
}


