import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { ITaskMeasurement } from "./task-measurement.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class TaskMeasurementController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<ITaskMeasurement> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        task_id: 0,
        date: "",
        length: 0,
        length_unit_id: 0,
        width: 0,
        width_unit_id: 0,
        height: 0,
        height_unit_id: 0,
        diameter: 0,
        diameter_unit_id: 0,
        volume: 0,
        volume_unit_id: 0,
        surface_area: 0,
        surface_area_unit_id: 0,
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

    const entitySchema = changeLog.entitySchema as Selectable<ITaskMeasurement>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.task_id !== "number") {
      entitySchema.task_id = 0;
    }

    if (typeof entitySchema.date !== "string") {
      entitySchema.date = "";
    }

    if (typeof entitySchema.length !== "number") {
      entitySchema.length = 0;
    }

    if (typeof entitySchema.length_unit_id !== "number") {
      entitySchema.length_unit_id = 0;
    }

    if (typeof entitySchema.width !== "number") {
      entitySchema.width = 0;
    }

    if (typeof entitySchema.width_unit_id !== "number") {
      entitySchema.width_unit_id = 0;
    }

    if (typeof entitySchema.height !== "number") {
      entitySchema.height = 0;
    }

    if (typeof entitySchema.height_unit_id !== "number") {
      entitySchema.height_unit_id = 0;
    }

    if (typeof entitySchema.diameter !== "number") {
      entitySchema.diameter = 0;
    }

    if (typeof entitySchema.diameter_unit_id !== "number") {
      entitySchema.diameter_unit_id = 0;
    }

    if (typeof entitySchema.volume !== "number") {
      entitySchema.volume = 0;
    }

    if (typeof entitySchema.volume_unit_id !== "number") {
      entitySchema.volume_unit_id = 0;
    }

    if (typeof entitySchema.surface_area !== "number") {
      entitySchema.surface_area = 0;
    }

    if (typeof entitySchema.surface_area_unit_id !== "number") {
      entitySchema.surface_area_unit_id = 0;
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
