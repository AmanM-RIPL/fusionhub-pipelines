import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IScheduleOfRatesLine } from "./schedule-of-rates-line.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class ScheduleOfRatesLineController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IScheduleOfRatesLine> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        schedule_id: 0,
        schedule_type_id: 0,
        cost: "",
        resource: "",
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

    const entitySchema = changeLog.entitySchema as Selectable<IScheduleOfRatesLine>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.schedule_id !== "number") {
      entitySchema.schedule_id = 0;
    }

    if (typeof entitySchema.schedule_type_id !== "number") {
      entitySchema.schedule_type_id = 0;
    }

    if (typeof entitySchema.cost !== "string") {
      entitySchema.cost = "";
    }

    if (typeof entitySchema.resource !== "string") {
      entitySchema.resource = "";
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
