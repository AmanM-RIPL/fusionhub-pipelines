#include "budget_head_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

BudgetHeadController::BudgetHeadController(QObject *parent)
    : QObject{parent},
    m_budgetheadRepository(RepositoryLocator::instance().budgetheadRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void BudgetHeadController::create(const QString &description) const
{
    BudgetHead budgethead;

    budgethead.setId(0);
    budgethead.setGlobalId("123");
    budgethead.setApprovalStatus(true);
    budgethead.setDescription(description);

    m_budgetheadRepository->saveQML(&budgethead);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = 0;
    jsonObject["globalId"] = "123";
    jsonObject["approvalStatus"] = true;
    jsonObject["description"] = description;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "BugdetHead::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getUserId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
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
    draftEntity.setEntity("BudgetHead");
    draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<BudgetHead*> BudgetHeadController::getBudgetHeadList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_budgetheadRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("BudgetHead");
        std::vector<BudgetHead*> budgetHeads;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto budgetHead = new BudgetHead();
                QJsonObject jsonObj = jsonDoc.object();
                budgetHead->setId(i + 1);
                budgetHead->setGlobalId("123");
                budgetHead->setApprovalStatus(true);
                budgetHead->setDescription(jsonObj["description"].toString());
                budgetHeads.push_back(budgetHead);
            }
        }
        return budgetHeads;
    }
}
