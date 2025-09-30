import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IMaterialIndent } from "./material-indent.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class MaterialIndentController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IMaterialIndent> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        material_id: 0,
        total_quantity: 0,
        task_id: null,
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

    const entitySchema = changeLog.entitySchema as Selectable<IMaterialIndent>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.material_id !== "number") {
      entitySchema.material_id = 0;
    }

    if (typeof entitySchema.total_quantity !== "number") {
      entitySchema.total_quantity = 0;
    }

    if (typeof entitySchema.task_id !== "number") {
      entitySchema.task_id = null;
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
