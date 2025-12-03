#include "good_received_note_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

GoodReceivedNoteController::GoodReceivedNoteController(QObject *parent)
    : QObject{parent},
    m_goodReceivedNoteRepository(RepositoryLocator::instance().goodReceivedNoteRepository()),
    //m_purchaseOrderLineRepository(RepositoryLocator::instance().purchaseOrderLineRepository()),
    //m_purchaseOrderRepository(RepositoryLocator::instance().purchaseOrderRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void GoodReceivedNoteController::create(const int &quantity, const int &purchaseOrderLineId) const
{
    GoodReceivedNote goodReceivedNote;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    goodReceivedNote.setId(id_in_milliseconds);
    goodReceivedNote.setGlobalId("123");
    goodReceivedNote.setApprovalStatus(true);
    goodReceivedNote.setQuantity(quantity);
    goodReceivedNote.setPurchaseOrderLineId(purchaseOrderLineId);

    m_goodReceivedNoteRepository->saveQML(&goodReceivedNote);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["quantity"] = quantity;
    jsonObject["purchaseOrderLineId"] = purchaseOrderLineId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "GoodReceivedNote::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("GoodReceivedNote");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}

std::vector<GoodReceivedNote*> GoodReceivedNoteController::getGoodReceivedNoteList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_goodReceivedNoteRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("GoodReceivedNote");
        std::vector<GoodReceivedNote*> goodReceivedNotes;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto goodReceivedNote = new GoodReceivedNote();
                QJsonObject jsonObj = jsonDoc.object();
                goodReceivedNote->setId(i + 1);
                goodReceivedNote->setGlobalId("123");
                goodReceivedNote->setApprovalStatus(true);

                goodReceivedNote->setQuantity(jsonObj["quantity"].toInt());
                goodReceivedNote->setPurchaseOrderLineId(jsonObj["purchaseOrderLineId"].toInt());

                goodReceivedNotes.push_back(goodReceivedNote);
            }
        }
        return goodReceivedNotes;
    }
}

/*std::vector<PurchaseOrderLine*> GoodReceivedNoteController::getPurchaseOrderLineList() const
{
    return m_purchaseOrderLineRepository->findAllQML();
}

std::vector<PurchaseOrder*> GoodReceivedNoteController::getPurchaseOrderList() const
{
    return m_purchaseOrderRepository->findAllQML();
}*/

