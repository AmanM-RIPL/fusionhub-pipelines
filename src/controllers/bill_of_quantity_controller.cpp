#include "bill_of_quantity_controller.h"
#include "schedule_of_rates_controller.h"
#include "schedule_of_rates_line_controller.h"
#include "bim_element_controller.h"
#include "common/repository_locator.h"
#include <QDir>

extern std::shared_ptr<User> gUser;
extern int gTenantId;
extern int gProjectId;

BillOfQuantityController::BillOfQuantityController(QObject *parent)
    : QObject{parent},
    m_billOfQuantityRepository(RepositoryLocator::instance().billOfQuantityRepository()),
    m_scheduleOfRatesRepository(RepositoryLocator::instance().scheduleOfRatesRepository()),
    m_draftEntityRepository(RepositoryLocator::instance().draftEntityRepository())
{}

void BillOfQuantityController::create(const QString &description, const int &scheduleOfRatesId) const
{
    /***********Start of DraftEntity******************/

    QJsonObject jsonObject;
    jsonObject["description"] = description;
    jsonObject["schedule_of_rates_id"] = scheduleOfRatesId;
    //jsonObject["bill_of_quantity_line"] = "NowBlanked";

    //For BOQLine
    std::vector<BillOfQuantity*> billOfQuantityList = getBillOfQuantityList(true);
    ScheduleOfRatesLineController scheduleOfRatesLineController;
    std::vector<ScheduleOfRatesLine*> scheduleOfRatesLineList = scheduleOfRatesLineController.getScheduleOfRatesLineList(true);

    BIMElementController bimElementController;
    std::vector<BIMElement*> bimElementList = bimElementController.getAllElements();

    for(int i = 0; i < billOfQuantityList.size(); i++ )
    {
        int id =  billOfQuantityList[i]->getId();
        if(scheduleOfRatesId == billOfQuantityList[i]->getScheduleOfRatesId())
        {
            for(int j = 0; j < scheduleOfRatesLineList.size(); j++)
            {
                int scheduleOfRatesLineId = scheduleOfRatesLineList[j]->getId();
                int scheduleSetupId = scheduleOfRatesLineList[j]->getScheduleSetupId();
                //int scheduleOfRateIdTemp = scheduleOfRatesLineList[j]->getScheduleOfRatesId();
                QString costParam = scheduleOfRatesLineList[j]->getCostParam();

                qDebug()<<"scheduleOfRatesLineId::"<<scheduleOfRatesLineId;
                qDebug()<<"scheduleSetupId::"<<scheduleSetupId;
                qDebug()<<"costParam::"<< costParam;
                QJsonArray dataArray;
                for(int k=0; k < bimElementList.size(); k++)
                {
                    BIMElement* bimElement =  bimElementList[k];

                    QList<BIMParameter*> bimParamentList =  bimElement->getParameterList();
                    for(int l=0; l < bimParamentList.size(); l++)
                    {
                        if(bimParamentList[l]->getKey() == "ScheduleSetup" && bimParamentList[l]->getValue().toInt() == scheduleSetupId)
                        {
                            int bimElementId = bimParamentList[l]->getBimElementId();
                            int lengthCostValue = bimElementController.getLengthValue(costParam);
                            int volumeCostValue = bimElementController.getVolumeValue(costParam);
                            int areaCostValue = bimElementController.getAreaValue(costParam);
                            int countCostValue = bimElementController.getCountValue(costParam);
                            int weightCostValue = bimElementController.getWeightValue(costParam);
                            int timeCostValue = bimElementController.getTimeValue(costParam);
                            int temperatureCostValue = bimElementController.getTemperatureValue(costParam);



                            //QString jsonString = costParam;
                            //QByteArray jsonData = jsonString.toUtf8();
                            //QJsonParseError error;
                            //QJsonDocument docCost = QJsonDocument::fromJson(jsonData, &error);
                            //jsonObject["bill_of_quantity_line"] = docCost.object();

                            //QJsonArray dataArray;

                            // Define your dynamic data (e.g., from a database or list)
                            // QList<QPair<QString, int>> items = {
                            //     {"1", 10},
                            //     {"3", 15},
                            //     {"4", 15}
                            // };

                            //for (const auto& item : items) {
                                QJsonObject element;
                                element["bimElementId"] = bimElementId;
                                element["totalCostValue"] = lengthCostValue
                                                            + volumeCostValue
                                                            + areaCostValue
                                                            + countCostValue
                                                            + weightCostValue
                                                            + timeCostValue
                                                            + temperatureCostValue;

                                dataArray.append(element);
                            //}

                            QJsonObject root;
                            root["data"] = dataArray;
                            root["row"] = dataArray.size();

                            QJsonDocument doc(root);
                            jsonObject["bill_of_quantity_line"] = doc.object();
                            jsonObject["schedule_setup_id"] = scheduleSetupId;
                        }
                    }
                }
            }
        }
    }

    //end of BOQLine



    QJsonDocument jsonDoc(jsonObject);
    QString entitySchema = jsonDoc.toJson(QJsonDocument::Indented);
    qDebug() << "BillOfQuantity::EntitySchema: " << entitySchema;

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
    draftEntity.setEntity("BillOfQuantity");
    draftEntity.setCreatedByUser(gUser->getId());
    draftEntity.setNextApprovingUser(gUser->getId());
    draftEntity.setEntitySchema(entitySchema);
    draftEntity.setAssociatedApprovedEntity(0);
    draftEntity.setChangeHistory(changeHistory);

    m_draftEntityRepository->saveQML(&draftEntity);


}

void BillOfQuantityController::approvedCreate( const int sorId, QString &description) const
{
    BillOfQuantity billOfQuantity;

    qint64 id_in_milliseconds = QDateTime::currentMSecsSinceEpoch();
    billOfQuantity.setId(id_in_milliseconds);
    billOfQuantity.setGlobalId("123");
   // billOfQuantity.setApprovalStatus(true);
    billOfQuantity.setDescription(description);
    billOfQuantity.setScheduleOfRatesId(sorId);

    m_billOfQuantityRepository->saveQML(&billOfQuantity);

}

std::vector<BillOfQuantity*> BillOfQuantityController::getBillOfQuantityList(bool isApproved) const
{
    qDebug()<<"IsApproved: "<< isApproved;

    if(isApproved){
        return m_billOfQuantityRepository->findAllQML();
    }
    else{
        std::vector<DraftEntity*>  draftEntitys  =  m_draftEntityRepository->findAllQML("BillOfQuantity");
        std::vector<BillOfQuantity*> billOfQuantitys;
        for(int i = 0; i < draftEntitys.size(); i++)
        {
            int draftId = draftEntitys[i]->getId();
            QString  jsonString = draftEntitys[i]->getEntitySchema();
            QString approvalStatus = draftEntitys[i]->getApprovalStatus();
            int nextApprovingUser = draftEntitys[i]->getNextApprovingUser();
            int createdByUser = draftEntitys[i]->getCreatedByUser();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            if (!jsonDoc.isNull() && jsonDoc.isObject())
            {
                auto billOfQuantity = new BillOfQuantity();
                QJsonObject jsonObj = jsonDoc.object();
                billOfQuantity->setId(draftId);
                billOfQuantity->setGlobalId("123");
                billOfQuantity->setApprovalStatus(approvalStatus);
                billOfQuantity->setCreatedByUser(createdByUser);
                billOfQuantity->setNextApprovingUser(nextApprovingUser);

                billOfQuantity->setDescription(jsonObj["description"].toString());
                billOfQuantity->setScheduleOfRatesId(jsonObj["schedule_of_rates_id"].toInt());

                billOfQuantitys.push_back(billOfQuantity);
            }
        }
        return billOfQuantitys;
    }
}

std::vector<ScheduleOfRates*> BillOfQuantityController::getScheduleOfRatesList() const
{
    return m_scheduleOfRatesRepository->findAllQML();
}


// #include <QJsonObject>
// #include <QJsonArray>
// #include <QJsonDocument>
// #include <QDebug>

void createBoqJson() {
    // 1. Create the 'data' array and its objects
    QJsonArray dataArray;

    // Define your dynamic data (e.g., from a database or list)
    QList<QPair<QString, int>> items = {
        {"1", 10},
        {"3", 15},
        {"4", 15}
    };

    for (const auto& item : items) {
        QJsonObject element;
        element["bimElementId"] = item.first;
        element["totalCostValue"] = item.second;
        dataArray.append(element);
    }

    // 2. Create the root object
    QJsonObject root;
    root["description"] = "boq1";
    root["schedule_of_rates_id"] = 1;
    root["schedule_setup_id"] = 1;
    root["data"] = dataArray;
    root["row"] = dataArray.size(); // Dynamically set row count

    // 3. Convert to JSON string
    QJsonDocument doc(root);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);

    qDebug() << bytes;
}


