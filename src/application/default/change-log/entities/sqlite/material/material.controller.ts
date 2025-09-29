import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IMaterial } from "./material.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class MaterialController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    // Fallback if entitySchema is invalid
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IMaterial> = {
        id: 0,
        global_id: 0,
        approval_status: false,
        material_name: "",
        category: "",
        unit_of_measurement_id: 0,
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

    const entitySchema = changeLog.entitySchema as Selectable<IMaterial>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = false;
    }

    if (typeof entitySchema.material_name !== "string") {
      entitySchema.material_name = "";
    }

    if (typeof entitySchema.category !== "string") {
      entitySchema.category = "";
    }

    if (typeof entitySchema.unit_of_measurement_id !== "number") {
      entitySchema.unit_of_measurement_id = 0;
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
