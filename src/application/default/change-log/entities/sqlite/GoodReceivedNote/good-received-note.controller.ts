import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IGoodReceivedNote } from "./good-received-note.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class GoodReceivedNoteController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IGoodReceivedNote> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        purchase_order_line_id: 0,
        amount_of_material_received: 0,
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

    const entitySchema = changeLog.entitySchema as Selectable<IGoodReceivedNote>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.purchase_order_line_id !== "number") {
      entitySchema.purchase_order_line_id = 0;
    }

    if (typeof entitySchema.amount_of_material_received !== "number") {
      entitySchema.amount_of_material_received = 0;
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
