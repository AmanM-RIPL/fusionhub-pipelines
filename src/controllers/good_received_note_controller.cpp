#include "good_received_note_controller.h"
#include "purchase_order_line_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

GoodReceivedNoteController::GoodReceivedNoteController(QObject *parent)
    : QObject{parent},
    m_goodReceivedNoteRepository(RepositoryLocator::instance().goodReceivedNoteRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void GoodReceivedNoteController::create(const int &purchaseOrderLineId, const int &quantity ) const
{
    // GoodReceivedNote goodReceivedNote;

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // goodReceivedNote.setId(id_in_milliseconds);
    // goodReceivedNote.setGlobalId("123");
    // goodReceivedNote.setApprovalStatus(true);
    // goodReceivedNote.setQuantity(quantity);
    // goodReceivedNote.setPurchaseOrderLineId(purchaseOrderLineId);

    // m_goodReceivedNoteRepository->saveQML(&goodReceivedNote);

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

void GoodReceivedNoteController::update(int id, const int &purchaseOrderLineId, const int &quantity ) const
{
    // GoodReceivedNote goodReceivedNote;

    // qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    // goodReceivedNote.setId(id_in_milliseconds);
    // goodReceivedNote.setGlobalId("123");
    // goodReceivedNote.setApprovalStatus(true);
    // goodReceivedNote.setQuantity(quantity);
    // goodReceivedNote.setPurchaseOrderLineId(purchaseOrderLineId);

    // m_goodReceivedNoteRepository->saveQML(&goodReceivedNote);

    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = id;
    jsonObject["quantity"] = quantity;
    jsonObject["purchaseOrderLineId"] = purchaseOrderLineId;


    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "GoodReceivedNote::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] = isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
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
    draftEntity.setEntity("GoodReceivedNote");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);

}
std::vector<GoodReceivedNote*> GoodReceivedNoteController::getGoodReceivedNoteList(bool isApproved) const
{
    qDebug() << "IsApproved: " << isApproved;

    if (isApproved)
    {
        return m_goodReceivedNoteRepository->findAllQML();
    } else{

        // Fetch draft GRN data
        std::vector<DraftEntity*> draftEntities = m_draftEntityRepository->findAllQML("GoodReceivedNote");

        // Reference data
        PurchaseOrderLineController workOrderLineController;
        std::vector<PurchaseOrderLine*> vecPurchaseOrderLine = workOrderLineController.getPurchaseOrderLineList(true);

        std::vector<GoodReceivedNote*> goodReceivedNotes;

        for(int i = 0; i < draftEntities.size(); i++)
        {
            int draftId = draftEntities[i]->getId();
            QString  jsonString = draftEntities[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());

            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                QJsonObject jsonObj = jsonDoc.object();

                int purchaseOrderLineId = jsonObj["purchaseOrderLineId"].toInt();
                int quantity = jsonObj["quantity"].toInt();

                // Create GRN object
                GoodReceivedNote* goodReceivedNote = new GoodReceivedNote();
                goodReceivedNote->setQuantity(quantity);
                goodReceivedNote->setId(draftId);
                goodReceivedNote->setGlobalId("123");

                foreach(const PurchaseOrderLine *po, vecPurchaseOrderLine)
                {
                    if (po->getId() == purchaseOrderLineId)
                    {
                        goodReceivedNote->setPurchaseOrderLineId(po->getId());
                        break;
                    }
                }
                goodReceivedNotes.push_back(goodReceivedNote);
            }
        }

        return goodReceivedNotes;}
}
