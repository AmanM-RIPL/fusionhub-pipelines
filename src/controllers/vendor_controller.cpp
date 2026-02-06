#include "vendor_controller.h"
#include "common/repository_locator.h"
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

VendorController::VendorController(QObject *parent)
    : QObject(parent),
    m_vendorRepository(RepositoryLocator::instance().vendorRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void VendorController::create(const QString &vendorName, const QString &vendorAddress, const QString &vendorContactPerson,
                              const QString &vendorMobile, const QString &vendorEmail) const
{
    QJsonObject jsonObject;
    //jsonObject["id"] = 0;
    //jsonObject["globalId"] = "123";
    //jsonObject["approvalStatus"] = "Pending";
    jsonObject["vendorName"] = vendorName;
    jsonObject["vendorAddress"] = vendorAddress;
    jsonObject["vendorMobile"] = vendorMobile;
    jsonObject["vendorEmail"] = vendorEmail;
    jsonObject["vendorContactPerson"] = vendorContactPerson;

    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "Vendor::EntitySchema: " << entitySchema;

    QDateTime currentDateTimeUtc = QDateTime::currentDateTimeUtc();
    QString isoDateTimeString = currentDateTimeUtc.toString(Qt::ISODateWithMs);

    QJsonObject jsonObjectChangeHistory;
  //  jsonObjectChangeHistory["user"] = gUser->getUserId();
    jsonObjectChangeHistory["user"] = gUser->getId();
    jsonObjectChangeHistory["timestamp"] =  isoDateTimeString;
    jsonObjectChangeHistory["changeType"] =  "create";
    jsonObjectChangeHistory["description"] = "Cretaed By User";
    jsonObjectChangeHistory["approvalHistory"] = "null";

    QJsonDocument jsonDocChangeHistory(jsonObjectChangeHistory);
    QString changeHistory = jsonDocChangeHistory.toJson(QJsonDocument::Indented);
    qDebug() << "Vendor::ChangeHistory: " << changeHistory;

    QDate createdOn = QDate::currentDate();

    DraftEntity draftEntity;
    draftEntity.setId(0);
    draftEntity.setTenant(gTenantId);
    draftEntity.setCreatedOn(createdOn);
    draftEntity.setProject(gProjectId);
    draftEntity.setEntity("Vendor");
    //draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
}


void VendorController::approvedCreate(const QString &vendorName, const QString &vendorAddress, const QString &vendorContactPerson,
                              const QString &vendorMobile, const QString &vendorEmail) const
{
    Vendor vendor;
    //qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();

    vendor.setId(0);
    vendor.setGlobalId("123");
   // vendor.setApprovalStatus("Pending");
    vendor.setVendorName(vendorName);
    vendor.setVendorAddress(vendorAddress);
    vendor.setVendorContactPerson(vendorContactPerson);
    vendor.setVendorMobile(vendorMobile);
    vendor.setVendorEmail(vendorEmail);

    m_vendorRepository->saveQML(&vendor);

}

void VendorController::update(int id, const QString &vendorName, const QString &vendorAddress, const QString &vendorContactPerson,
                              const QString &vendorMobile, const QString &vendorEmail) const
{
    QJsonObject jsonObject;
    jsonObject["vendorName"] = vendorName;
    jsonObject["vendorAddress"] = vendorAddress;
    jsonObject["vendorMobile"] = vendorMobile;
    jsonObject["vendorEmail"] = vendorEmail;
    jsonObject["vendorContactPerson"] = vendorContactPerson;

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
    draftEntity.setEntity("Vendor");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->updateQML(&draftEntity);
}

std::vector<Vendor*> VendorController::getVendorList(bool isApproved) const
{    
    qDebug()<<"IsApproved: "<< isApproved;    

    if(isApproved){
        return m_vendorRepository->findAllQML();
    }
    else{        
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("Vendor");
        std::vector<Vendor*> vendors;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            int draftId = draftEntitys[i]->getId();
            QString approvalStatus = draftEntitys[i]->getApprovalStatus();
            int nextApprovingUser = draftEntitys[i]->getNextApprovingUser();
            int createdByUser = draftEntitys[i]->getCreatedByUser();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto vendor = new Vendor();
                QJsonObject jsonObj = jsonDoc.object();
                vendor->setId(draftId);
                vendor->setGlobalId("123");
                vendor->setApprovalStatus(approvalStatus);
                vendor->setCreatedByUser(createdByUser);
                vendor->setNextApprovingUser(nextApprovingUser);
                vendor->setVendorName(jsonObj["vendorName"].toString());
                vendor->setVendorAddress(jsonObj["vendorAddress"].toString());
                vendor->setVendorContactPerson(jsonObj["vendorContactPerson"].toString());
                vendor->setVendorMobile(jsonObj["vendorMobile"].toString());
                vendor->setVendorEmail(jsonObj["vendorEmail"].toString());

                vendors.push_back(vendor);
            }
        }
        return vendors;
    }
}


