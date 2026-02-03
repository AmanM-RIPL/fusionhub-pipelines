#include "background_thread_manager.h"
#include "network/network_manager.h"
#include <QDebug>
#include "common/repository_locator.h"

#include "controllers/material_controller.h"
#include "controllers/bill_of_quantity_controller.h"
#include "controllers/bill_of_quantity_line_controller.h"
#include "controllers/bim_element_controller.h"
#include "controllers/budget_head_controller.h"
#include "controllers/file_controller.h"
#include "controllers/good_received_note_controller.h"
#include "controllers/material_indent_controller.h"
#include "controllers/project_budget_controller.h"
#include "controllers/purchase_order_controller.h"
#include "controllers/purchase_order_line_controller.h"
#include "controllers/schedule_of_rates_controller.h"
#include "controllers/schedule_of_rates_line_controller.h"
#include "controllers/schedule_setup_controller.h"
#include "controllers/task_controller.h"
#include "controllers/unit_of_measurement_controller.h"
#include "controllers/user_controller.h"
#include "controllers/vendor_controller.h"
#include "controllers/work_billing_controller.h"
#include "controllers/work_billing_line_controller.h"
#include "controllers/work_order_controller.h"
#include "controllers/work_order_line_controller.h"

BackgroundThreadManager* BackgroundThreadManager::m_instance = nullptr;

BackgroundThreadManager::BackgroundThreadManager(QObject *parent)
    : QObject(parent)
{
}

BackgroundThreadManager* BackgroundThreadManager::instance()
{
    if (!m_instance)
    {
        m_instance = new BackgroundThreadManager();
    }
    return m_instance;
}

void BackgroundThreadManager::runBackgroundTaskForDraftDataSync()
{
    auto* repo = RepositoryLocator::instance().draftEntityRepository();
    auto drafts = repo->findAllApprovedQML();
    if (drafts.empty()) {
        qDebug() << "No draft data found.";
        return;
    }
    auto* network = NetworkManager::getInstance();
    for (auto* draft : drafts)
    {
        if (!draft) continue;
        QJsonObject payload;
        payload["project"] = draft->getProject();
        payload["entity"]  = draft->getEntity();
        payload["createdOn"] = draft->getCreatedOn().toString(Qt::ISODate);
        payload["createdByUser"] = draft->getCreatedByUser();
        payload["nextApprovingUser"] = draft->getNextApprovingUser();
        payload["associatedApprovedEntity"] = draft->getAssociatedApprovedEntity();
        // -------- entitySchema ----------
        QJsonDocument schemaDoc = QJsonDocument::fromJson(draft->getEntitySchema().toUtf8() );
        payload["entitySchema"] = schemaDoc.object();
        // -------- changeHistory ----------
        QJsonDocument historyDoc = QJsonDocument::fromJson(draft->getChangeHistory().toUtf8());
        payload["changeHistory"] = historyDoc.object();
        // qDebug() << "Sending payload:\n"
        //          << QJsonDocument(payload)
        //                 .toJson(QJsonDocument::Indented);
        network->sendDraftToServer(payload);
    }
    if (repo->deleteAllApprovedQML()) {
        qDebug() << "Approved draft entities deleted after successful sync.";
    } else {
        qDebug() << "Failed to delete approved draft entities after sync.";
    }
}

void BackgroundThreadManager::runBackgroundTaskForChangeLogSync()
{
    auto* network = NetworkManager::getInstance();
    // Connect signal to process when data arrives
    connect(network, &NetworkManager::changeLogSyncReceived, this,
            &BackgroundThreadManager::onChangeLogSyncReceived, Qt::UniqueConnection);
    // Request the changelog (data will arrive in onChangeLogSyncReceived)
    network->requestChangeLogSync();
}


void BackgroundThreadManager::onChangeLogSyncReceived(const QJsonArray &arr)
{
    for (const auto &v : arr) {
        if (!v.isObject()) continue;

        QJsonObject obj = v.toObject();
        QString entity = obj["entity"].toString();
        QJsonObject changeHistory = obj["changeHistory"].toObject();
        QString changeType = changeHistory["changeType"].toString();

        QJsonObject schema = obj["entitySchema"].toObject();
        if (entity == "UnitOfMeasurement") {
            QString name = schema["uom_name"].toString();
            QString type = schema["unit_type"].toString();
            double sqm = schema["conversion_to_sqm"].toDouble();
            double cbm = schema["conversion_to_cubic_meter"].toDouble();
            double meter = schema["conversion_to_meter"].toDouble();
            double kg = schema["conversion_to_kilogram"].toDouble();

            UnitOfMeasurementController controller;
            controller.approvedCreate(name, type, sqm, cbm, meter, kg);

            qDebug() << "UOM Created:" << name;
        }

        else if (entity == "Vendor") {
            QString name = schema["vendor_name"].toString();
            QString address = schema["vendor_address"].toString();
            QString contactPerson = schema["vendor_contact_person"].toString();
            QString mobile = schema["vendor_mobile"].toString();
            QString email = schema["vendor_email"].toString();

            VendorController controller;
            controller.approvedCreate(name, address, contactPerson, mobile, email);

            qDebug() << "Vendor Created:" << name;
        }

        else if (entity == "Material") {
            QString name = schema["material_name"].toString();
            QString category = schema["category"].toString();
            int unitOfMeasurementId = schema["unit_of_measurement_id"].toInt();

            MaterialController materialController;
            materialController.approvedCreate(name, category, unitOfMeasurementId);

            qDebug() << "Material Created:" << name << category << unitOfMeasurementId;
        }

        else if (entity == "BudgetHead") {
            QString description = schema["description"].toString();

            BudgetHeadController controller;
            controller.approvedCreate(description);

            qDebug() << "BudgetHead Created:" << description;
        }

        else if (entity == "ScheduleSetup") {
            QString name = schema["schedule_setup_name"].toString();
            QString description = schema["description"].toString();
            QString costParam = schema["cost_parameter"].toString();
            QString resourceParam = schema["resource_parameter"].toString();

            ScheduleSetupController controller;
            controller.approvedCreate(name, description, costParam, resourceParam);

            qDebug() << "ScheduleSetup Created:" << name;
        }

        else if (entity == "Task") {
            QString name = schema["task_name"].toString();
            QString description = schema["description"].toString();
            QString bimElement = schema["bim_element"].toString();
            QString startDate = schema["start_date"].toString();
            QString endDate = schema["end_date"].toString();
            int pid = schema["pid"].toInt();
            QString status = schema["status"].toString();

            TaskController controller;
            controller.approvedCreate(name, description, bimElement,
                                      startDate, endDate, pid, status);

            qDebug() << "Task Created:" << name;
        }
        else if (entity == "File") {
            QString description = schema["description"].toString();
            QString url = schema["file_url"].toString();
            QString localPath = schema["file_local_path"].toString();

            FileController controller;
            controller.approvedCreate(description, url, localPath);

            qDebug() << "File Created:" << description;
        }
        else if (entity == "ScheduleOfRates") {
            QString name = schema["schedule_of_rates_name"].toString();

            ScheduleOfRatesController controller;
            controller.approvedCreate(name);

            qDebug() << "ScheduleOfRates Created:" << name;
        }

        else if (entity == "ScheduleOfRatesLine") {
            int setupId = schema["schedule_setup_id"].toInt();
            int sorId = schema["schedule_of_rates_id"].toInt();
            QString cost = schema["cost"].toString();
            QString resource = schema["resource"].toString();

            ScheduleOfRatesLineController controller;
            controller.approvedCreate(setupId, sorId, cost, resource);

            qDebug() << "ScheduleOfRatesLine Created";
        }

        else if (entity == "BillOfQuantity") {
            int sorId = schema["schedule_of_rates_id"].toInt();
            QString description = schema["description"].toString();

            BillOfQuantityController controller;
            controller.approvedCreate(sorId, description);

            qDebug() << "BOQ Created";
        }

        else if (entity == "BillOfQuantityLine") {
            int boqId = schema["bill_of_quantity_id"].toInt();
            QString description = schema["description"].toString();
            double value = schema["dollar_value"].toDouble();
            int bimElementId = schema["bim_element_id"].toInt();

            BillOfQuantityLineController controller;
            controller.approvedCreate(boqId, description, value, bimElementId);

            qDebug() << "BOQ Line Created";
        }

        else if (entity == "ProjectBudget") {
            int budgetHeadId = schema["budget_head_id"].toInt();
            QString value = schema["dollar_value"].toString();

            ProjectBudgetController controller;
            controller.approvedCreate(budgetHeadId, value);

            qDebug() << "ProjectBudget Created";
        }

        else if (entity == "WorkOrder") {
            int vendorId = schema["vendor_id"].toInt();
            QString workOrderName = schema["description"].toString();

            WorkOrderController controller;
            controller.approvedCreate(vendorId, workOrderName);

            qDebug() << "WorkOrder Created " << workOrderName;
        }

        else if (entity == "WorkOrderLine") {
            int workOrderId = schema["work_order_id"].toInt();
            QString description = schema["description"].toString();
            double value = schema["dollar_value"].toDouble();
            double tax = schema["tax_amount"].toDouble();
            double withholding = schema["tax_withholding_amount"].toDouble();
            int taskId = schema["task_id"].toInt();
            double retention = schema["retention_amount"].toDouble();

            WorkOrderLineController controller;
            controller.approvedCreate(workOrderId, description, value,
                                      tax, withholding, taskId, retention);

            qDebug() << "WorkOrderLine Created";
        }

        else if (entity == "WorkBilling") {
            int workOrderId = schema["work_order_id"].toInt();
            QString workBillingName = "workBillingName";
            WorkBillingController controller;
            controller.approvedCreate(workBillingName, workOrderId);

            qDebug() << "WorkBilling Created";
        }

        else if (entity == "WorkBillingLine") {
            int lineId = schema["work_order_line_id"].toInt();
            double value = schema["dollar_value"].toDouble();
            double tax = schema["tax_amount"].toDouble();
            double withholding = schema["tax_withholding_amount"].toDouble();
            double retention = schema["retention_amount"].toDouble();

            WorkBillingLineController controller;
            controller.approvedCreate(lineId, value, tax, withholding, retention);

            qDebug() << "WorkBillingLine Created";
        }

        else if (entity == "PurchaseOrder") {
            int vendorId = schema["vendor_id"].toInt();

            PurchaseOrderController controller;
            controller.approvedCreate(vendorId);

            qDebug() << "PurchaseOrder Created";
        }

        else if (entity == "PurchaseOrderLine") {
            int poId = schema["purchase_order_id"].toInt();
            int materialId = schema["material_id"].toInt();
            double qty = schema["quantity"].toDouble();
            int uomId = schema["unit_of_measurement_id"].toInt();
            double value = schema["dollar_value"].toDouble();
            double tax = schema["tax_amount"].toDouble();
            double withholding = schema["tax_withholding"].toDouble();

            PurchaseOrderLineController controller;
            controller.approvedCreate(poId, materialId, qty, uomId, value, tax, withholding);

            qDebug() << "PurchaseOrderLine Created";
        }

        else if (entity == "GoodReceivedNote") {
            int purchaseOrderLineId = schema["purchase_order_line_id"].toInt();
            double quantity = schema["amount_of_material_received"].toDouble();

            GoodReceivedNoteController controller;
            controller.approvedCreate(purchaseOrderLineId, quantity);

            qDebug() << "GRN Created";
        }

        else if (entity == "MaterialIndent") {
            int materialId = schema["material_id"].toInt();
            double quantity = schema["total_quantity"].toDouble();
            int taskId = schema["task_id"].toInt();

            MaterialIndentController controller;
            controller.approvedCreate(quantity, materialId, taskId);

            qDebug() << "MaterialIndent Created" << quantity;
        }


    }

}
