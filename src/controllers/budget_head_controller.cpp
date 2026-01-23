#include "budget_head_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

BudgetHeadController::BudgetHeadController(QObject *parent)
    : QObject{parent},
    m_budgetHeadRepository(RepositoryLocator::instance().budgetHeadRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void BudgetHeadController::create(const QString &description) const
{
    BudgetHead budgetHead;

    budgetHead.setId(0);
    budgetHead.setGlobalId("123");
    budgetHead.setApprovalStatus(true);
    budgetHead.setDescription(description);

    m_budgetHeadRepository->saveQML(&budgetHead);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = true;
    jsonObject["description"] = description;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "BugdetHead::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    //jsonObjectChangeHistory["user"] = gUser->getUserId();
    jsonObjectChangeHistory["user"] = gUser->getId();
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
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


void BudgetHeadController::update(int id, const QString &description) const
{
    QJsonObject jsonObject;
    jsonObject["description"] = description;
    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    // qDebug() << "PurchaseOrder:EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("Material");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}
std::vector<BudgetHead*> BudgetHeadController::getBudgetHeadList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_budgetHeadRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("BudgetHead");
        std::vector<BudgetHead*> budgetHeads;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
             int draftId = draftEntitys[i]->getId();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto budgetHead = new BudgetHead();
                QJsonObject jsonObj = jsonDoc.object();
                budgetHead->setId(draftId);
                budgetHead->setGlobalId("123");
                budgetHead->setApprovalStatus(true);
                budgetHead->setDescription(jsonObj["description"].toString());
                budgetHeads.push_back(budgetHead);
            }
        }
        return budgetHeads;
    }
}
