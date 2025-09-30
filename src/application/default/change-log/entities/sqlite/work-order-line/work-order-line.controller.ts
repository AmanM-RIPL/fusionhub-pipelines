import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IWorkOrderLine } from "./work-order-line.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class WorkOrderLineController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IWorkOrderLine> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        work_order_id: 0,
        description: "",
        dollar_value: 0,
        tax_amount: 0,
        tax_withholding_amount: 0,
        task_id: null,
        retention_amount: 0,
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

    const entitySchema = changeLog.entitySchema as Selectable<IWorkOrderLine>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.work_order_id !== "number") {
      entitySchema.work_order_id = 0;
    }

    if (typeof entitySchema.description !== "string") {
      entitySchema.description = "";
    }

    if (typeof entitySchema.dollar_value !== "number") {
      entitySchema.dollar_value = 0;
    }

    if (typeof entitySchema.tax_amount !== "number") {
      entitySchema.tax_amount = 0;
    }

    if (typeof entitySchema.tax_withholding_amount !== "number") {
      entitySchema.tax_withholding_amount = 0;
    }

    if (typeof entitySchema.task_id !== "number") {
      entitySchema.task_id = null;
    }

    if (typeof entitySchema.retention_amount !== "number") {
      entitySchema.retention_amount = 0;
    }

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
