import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IPurchaseOrderLine } from "./purchase-order-line.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class PurchaseOrderLineController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IPurchaseOrderLine> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        purchase_order_id: 0,
        material_id: 0,
        quantity: 0,
        unit_of_measurement_id: 0,
        dollar_value: 0,
        tax_amount: 0,
        tax_withholding: 0,
        change_history: {
          user: 0,
          changeType: "create",
          description: "",
          timestamp: new Date(),
          approvalHistory: []
        }
      };

      changeLog.entitySchema = newEntitySchema;
    }

    const entitySchema = changeLog.entitySchema as Selectable<IPurchaseOrderLine>;

      if (typeof entitySchema.global_id !== "number") {
          entitySchema.global_id = 0
      };
      
      if (typeof entitySchema.approval_status !== "boolean") {
          entitySchema.approval_status = true
      };
      if (typeof entitySchema.purchase_order_id !== "number") {
          entitySchema.purchase_order_id = 0
      };
      if (typeof entitySchema.material_id !== "number") {
          entitySchema.material_id = 0
      };
      if (typeof entitySchema.quantity !== "number") {
          entitySchema.quantity = 0
      };
      if (typeof entitySchema.unit_of_measurement_id !== "number") {
          entitySchema.unit_of_measurement_id = 0
      };
      if (typeof entitySchema.dollar_value !== "number") {
          entitySchema.dollar_value = 0
      };
      if (typeof entitySchema.tax_amount !== "number") {
          entitySchema.tax_amount = 0
      };
      if (typeof entitySchema.tax_withholding !== "number") {
          entitySchema.tax_withholding = 0
      };

    if (typeof entitySchema.change_history !== "object" || entitySchema.change_history === null) {
      entitySchema.change_history = {
        user: 0,
        changeType: "create",
        description: "",
        timestamp: new Date(),
        approvalHistory: []
      } as ChangeHistory;
    }

    changeLog.entitySchema = entitySchema;
  }
}
