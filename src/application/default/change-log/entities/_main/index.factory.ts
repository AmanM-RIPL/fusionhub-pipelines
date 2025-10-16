import { Database as DatabaseType } from "better-sqlite3";
import { IProjectEntityBaseController, IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import UnitOfMeasurementDao from "../sqlite/unit-of-measurement/unit-of-measurement.dao";
import { UnitOfMeasurementController } from "../sqlite/unit-of-measurement/unit-of-measurement.controller";
import VendorDao from "../sqlite/vendor/vendor.dao";
import MaterialDao from "../sqlite/material/material.dao";
import BudgetHeadDao from "../sqlite/budget-head/budget-head.dao";
import ProjectBudgetDao from "../sqlite/project-budget/project-budget.dao";
import ScheduleSetupDao from "../sqlite/schedule-setup/schedule-setup.dao";
import ScheduleOfRatesDao from "../sqlite/schedule-of-rates/schedule-of-rates.dao";
import ScheduleOfRatesLineDao from "../sqlite/schedule-of-rates-line/schedule-of-rates-line.dao";
import TaskDao from "../sqlite/task/task.dao";
import TaskImageDao from "../sqlite/task-image/task-image.dao";
import TaskMeasurementDao from "../sqlite/task-measurement/task-measurement.dao";
import BillOfQuantityDao from "../sqlite/bill-of-quantity/bill-of-quantity.dao";
import BillOfQuantityLineDao from "../sqlite/bill-of-quantity-line/bill-of-quantity-line.dao";
import FileDao from "../sqlite/file/file.dao";
import FilePermissionDao from "../sqlite/file-permission/file-permission.dao";
import PurchaseOrderDao from "../sqlite/purchase-order/purchase-order.dao";
import PurchaseOrderLineDao from "../sqlite/purchase-order-line/purchase-order-line.dao";
import GoodReceivedNoteDao from "../sqlite/good-received-note/good-received-note.dao";
import MaterialIndentDao from "../sqlite/material-indent/material-indent.dao";
import WorkOrderDao from "../sqlite/work-order/work-order.dao";
import WorkOrderLineDao from "../sqlite/work-order-line/work-order-line.dao";
import WorkBillingDao from "../sqlite/work-billing/work-billing.dao";
import { VendorController } from "../sqlite/vendor/vendor.controller";
import { MaterialController } from "../sqlite/material/material.controller";
import { BudgetHeadController } from "../sqlite/budget-head/budget-head.controller";
import { ProjectBudgetController } from "../sqlite/project-budget/project-budget.controller";
import { ScheduleSetupController } from "../sqlite/schedule-setup/schedule-setup.controller";
import { ScheduleOfRatesController } from "../sqlite/schedule-of-rates/schedule-of-rates.controller";
import { ScheduleOfRatesLineController } from "../sqlite/schedule-of-rates-line/schedule-of-rates-line.controller";
import { TaskController } from "../sqlite/task/task.controller";
import { TaskImageController } from "../sqlite/task-image/task-image.controller";
import { TaskMeasurementController } from "../sqlite/task-measurement/task-measurement.controller";
import { BillOfQuantityController } from "../sqlite/bill-of-quantity/bill-of-quantity.controller";
import { BillOfQuantityLineController } from "../sqlite/bill-of-quantity-line/bill-of-quantity-line.controller";
import { FileController } from "../sqlite/file/file.controller";
import { FilePermissionController } from "../sqlite/file-permission/file-permission.controller";
import { PurchaseOrderController } from "../sqlite/purchase-order/purchase-order.controller";
import { PurchaseOrderLineController } from "../sqlite/purchase-order-line/purchase-order-line.controller";
import { GoodReceivedNoteController } from "../sqlite/good-received-note/good-received-note.controller";
import { MaterialIndentController } from "../sqlite/material-indent/material-indent.controller";
import { WorkOrderController } from "../sqlite/work-order/work-order.controller";
import { WorkOrderLineController } from "../sqlite/work-order-line/work-order-line.controller";
import { WorkBillingController } from "../sqlite/work-billing/work-billing.controller";
import { WorkBillingLineController } from "../sqlite/work-billing-line/work-billing-line.controller";
import WorkBillingLineDao from "../sqlite/work-billing-line/work-billing-line.dao";

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
      return new WorkBillingLineDao(db);

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
