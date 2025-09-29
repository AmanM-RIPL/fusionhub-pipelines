import { Database as DatabaseType } from "better-sqlite3";
import { IProjectEntityBaseController, IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import UnitOfMeasurementDao from "../sqlite/UnitOfMeasurement/unit-of-measurement.dao";
import { UnitOfMeasurementController } from "../sqlite/UnitOfMeasurement/unit-of-measurement.controller";
import VendorDao from "../sqlite/vendor/vendor.dao";
import MaterialDao from "../sqlite/material/material.dao";
import BudgetHeadDao from "../sqlite/BudgetHead/budget-head.dao";
import ProjectBudgetDao from "../sqlite/ProjectBudget/project-budget.dao";
import ScheduleSetupDao from "../sqlite/ScheduleSetup/schedule-setup.dao";
import ScheduleOfRatesDao from "../sqlite/ScheduleOfRates/schedule-of-rates.dao";
import ScheduleOfRatesLineDao from "../sqlite/ScheduleOfRatesLine/schedule-of-rates-line.dao";
import TaskDao from "../sqlite/Task/task.dao";
import TaskImageDao from "../sqlite/TaskImage/task-image.dao";
import TaskMeasurementDao from "../sqlite/TaskMeasurement/task-measurement.dao";
import BillOfQuantityDao from "../sqlite/BillOfQuantity/bill-of-quantity.dao";
import BillOfQuantityLineDao from "../sqlite/BillOfQuantityLine/bill-of-quantity-line.dao";
import FileDao from "../sqlite/File/file.dao";
import FilePermissionDao from "../sqlite/FilePermission/file-permission.dao";
import PurchaseOrderDao from "../sqlite/PurchaseOrder/purchase-order.dao";
import PurchaseOrderLineDao from "../sqlite/PurchaseOrderLine/purchase-order-line.dao";
import GoodReceivedNoteDao from "../sqlite/GoodReceivedNote/good-received-note.dao";
import MaterialIndentDao from "../sqlite/MaterialIndent/material-indent.dao";
import WorkOrderDao from "../sqlite/WorkOrder/work-order.dao";
import WorkOrderLineDao from "../sqlite/WorkOrderLine/work-order-line.dao";
import WorkBillingDao from "../sqlite/WorkBilling/work-billing.dao";
import { VendorController } from "../sqlite/vendor/vendor.controller";
import { MaterialController } from "../sqlite/material/material.controller";
import { BudgetHeadController } from "../sqlite/BudgetHead/budget-head.controller";
import { ProjectBudgetController } from "../sqlite/ProjectBudget/project-budget.controller";
import { ScheduleSetupController } from "../sqlite/ScheduleSetup/schedule-setup.controller";
import { ScheduleOfRatesController } from "../sqlite/ScheduleOfRates/schedule-of-rates.controller";
import { ScheduleOfRatesLineController } from "../sqlite/ScheduleOfRatesLine/schedule-of-rates-line.controller";
import { TaskController } from "../sqlite/Task/task.controller";
import { TaskImageController } from "../sqlite/TaskImage/task-image.controller";
import { TaskMeasurementController } from "../sqlite/TaskMeasurement/task-measurement.controller";
import { BillOfQuantityController } from "../sqlite/BillOfQuantity/bill-of-quantity.controller";
import { BillOfQuantityLineController } from "../sqlite/BillOfQuantityLine/bill-of-quantity-line.controller";
import { FileController } from "../sqlite/File/file.controller";
import { FilePermissionController } from "../sqlite/FilePermission/file-permission.controller";
import { PurchaseOrderController } from "../sqlite/PurchaseOrder/purchase-order.controller";
import { PurchaseOrderLineController } from "../sqlite/PurchaseOrderLine/purchase-order-line.controller";
import { GoodReceivedNoteController } from "../sqlite/GoodReceivedNote/good-received-note.controller";
import { MaterialIndentController } from "../sqlite/MaterialIndent/material-indent.controller";
import { WorkOrderController } from "../sqlite/WorkOrder/work-order.controller";
import { WorkOrderLineController } from "../sqlite/WorkOrderLine/work-order-line.controller";
import { WorkBillingController } from "../sqlite/WorkBilling/work-billing.controller";
import { WorkBillingLineController } from "../sqlite/WorkBillingLine/work-billing-line.controller";

// get DAO based on entity name
export function getSQLiteDao(entityName: string, db: DatabaseType): IProjectEntityBaseRepository | undefined {
  switch (entityName) {
    case "UnitOfMeasurement":
      return new UnitOfMeasurementDao(db);
    case "Vendor":
      return new VendorDao(db);
    case "Material":
      return new MaterialDao(db);
    case "BudgetHead":
      return new BudgetHeadDao(db);
    case "ProjectBudget":
      return new ProjectBudgetDao(db);
    case "ScheduleSetup":
      return new ScheduleSetupDao(db);
    case "ScheduleOfRates":
      return new ScheduleOfRatesDao(db);
    case "ScheduleOfRatesLine":
      return new ScheduleOfRatesLineDao(db);
    case "Task":
      return new TaskDao(db);
    case "TaskImage":
      return new TaskImageDao(db);
    case "TaskMeasurement":
      return new TaskMeasurementDao(db);
    case "BillOfQuantity":
      return new BillOfQuantityDao(db);
    case "BillOfQuantityLine":
      return new BillOfQuantityLineDao(db);
    case "File":
      return new FileDao(db);
    case "FilePermission":
      return new FilePermissionDao(db);
    case "PurchaseOrder":
      return new PurchaseOrderDao(db);
    case "PurchaseOrderLine":
      return new PurchaseOrderLineDao(db);
    case "GoodReceivedNote":
      return new GoodReceivedNoteDao(db);
    case "MaterialIndent":
      return new MaterialIndentDao(db);
    case "WorkOrder":
      return new WorkOrderDao(db);
    case "WorkOrderLine":
      return new WorkOrderLineDao(db);
    case "WorkBilling":
      return new WorkBillingDao(db);
    case "WorkBillingLine":
      return new WorkBillingDao(db);
    default:
      return undefined;
  }

}

// get Controller based on entity name
export function getSqLiteController(entityName: string): IProjectEntityBaseController | undefined {
  switch (entityName) {
    case "UnitOfMeasurement":
      return new UnitOfMeasurementController();
    case "Vendor":
      return new VendorController();
    case "Material":
      return new MaterialController();
    case "BudgetHead":
      return new BudgetHeadController();
    case "ProjectBudget":
      return new ProjectBudgetController();
    case "ScheduleSetup":
      return new ScheduleSetupController();
    case "ScheduleOfRates":
      return new ScheduleOfRatesController();
    case "ScheduleOfRatesLine":
      return new ScheduleOfRatesLineController();
    case "Task":
      return new TaskController();
    case "TaskImage":
      return new TaskImageController();
    case "TaskMeasurement":
      return new TaskMeasurementController();
    case "BillOfQuantity":
      return new BillOfQuantityController();
    case "BillOfQuantityLine":
      return new BillOfQuantityLineController();
    case "File":
      return new FileController();
    case "FilePermission":
      return new FilePermissionController();
    case "PurchaseOrder":
      return new PurchaseOrderController();
    case "PurchaseOrderLine":
      return new PurchaseOrderLineController();
    case "GoodReceivedNote":
      return new GoodReceivedNoteController();
    case "MaterialIndent":
      return new MaterialIndentController();
    case "WorkOrder":
      return new WorkOrderController();
    case "WorkOrderLine":
      return new WorkOrderLineController();
    case "WorkBilling":
      return new WorkBillingController();
    case "WorkBillingLine":
      return new WorkBillingLineController();
    default:
      return undefined;
  }

}
