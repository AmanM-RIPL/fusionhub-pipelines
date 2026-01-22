#include "bill_of_quantity_line_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

BillOfQuantityLineController::BillOfQuantityLineController(QObject *parent)
    : QObject{parent},
    m_billOfQuantityLineRepository(RepositoryLocator::instance().billOfQuantityLineRepository()),
    m_billOfQuantityRepository(RepositoryLocator::instance().billOfQuantityRepository()),
    m_bimElementRepository(RepositoryLocator::instance().bimElementRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void BillOfQuantityLineController::create(const QString &description, const int &billOfQuantityId ,const int &bimElementId) const
{
    BillOfQuantityLine billOfQuantityLine;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    billOfQuantityLine.setId(id_in_milliseconds);
    billOfQuantityLine.setGlobalId("123");
    billOfQuantityLine.setApprovalStatus(true);
    billOfQuantityLine.setBillOfQuantityLineName(description);
    billOfQuantityLine.setBillOfQuantityId(billOfQuantityId);  
    billOfQuantityLine.setBimElementId(bimElementId);

    m_billOfQuantityLineRepository->saveQML(&billOfQuantityLine);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = true;
    jsonObject["description"] = description;
    jsonObject["bill_of_quantity_id"] = billOfQuantityId;
    jsonObject["bim_element_id"] = bimElementId;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "BillOfQuantityLine::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("BillOfQuantityLine");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<BillOfQuantityLine*> BillOfQuantityLineController::getBillOfQuantityLineList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_billOfQuantityLineRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("BillOfQuantityLine");
        std::vector<BillOfQuantityLine*> billOfQuantityLines;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto billOfQuantityLine = new BillOfQuantityLine();
                QJsonObject jsonObj = jsonDoc.object();
                billOfQuantityLine->setId(i + 1);
                billOfQuantityLine->setGlobalId("123");
                billOfQuantityLine->setApprovalStatus(true);

                billOfQuantityLine->setBillOfQuantityLineName(jsonObj["description"].toString());
                billOfQuantityLine->setBimElementId(jsonObj["bim_element_id"].toInt());
                billOfQuantityLine->setBillOfQuantityId(jsonObj["bill_of_quantity_id"].toInt());

                billOfQuantityLines.push_back(billOfQuantityLine);
            }
        }
        return billOfQuantityLines;
    }
}

std::vector<BillOfQuantity*> BillOfQuantityLineController::getBOQList() const
{
    return m_billOfQuantityRepository->findAllQML();
}

std::vector<BIMElement*> BillOfQuantityLineController::getBimElementList() const
{
    return m_bimElementRepository->findAllQML();
}
