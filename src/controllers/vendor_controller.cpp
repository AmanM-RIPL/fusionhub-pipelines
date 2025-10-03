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

    qDebug() <<"gTenantId:" << gTenantId;
    qDebug() <<"gProjectId:" << gProjectId;
    qDebug() <<"CurrentUserId:" << gUser->getUserId();


    Vendor vendor;
    //qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();

    vendor.setId(0);
    vendor.setGlobalId("123");
    vendor.setApprovalStatus(true);
    vendor.setVendorName(vendorName);
    vendor.setVendorAddress(vendorAddress);
    vendor.setVendorContactPerson(vendorContactPerson);
    vendor.setVendorMobile(vendorMobile);
    vendor.setVendorEmail(vendorEmail);

    m_vendorRepository->saveQML(&vendor);


    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["id"] = 0;
    jsonObject["globalId"] = "123";
    jsonObject["approvalStatus"] = true;
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
    jsonObjectChangeHistory["user"] = gUser->getUserId();
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
    draftEntity.setCreatedByUser(gUser->getUserId());
    draftEntity.setNextApprovingUser(0);
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);
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
          QString  jsonString = draftEntitys[i]->getEntitySchema();
          QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
          if (!jsonDoc.isNull() && jsonDoc.isObject())
          {
              auto vendor = new Vendor();
              QJsonObject jsonObj = jsonDoc.object();
              vendor->setId(i + 1);
              vendor->setGlobalId("123");
              vendor->setApprovalStatus(true);
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

