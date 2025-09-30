import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IScheduleOfRates } from "./schedule-of-rates.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class ScheduleOfRatesController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IScheduleOfRates> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        schedule_name: "",
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

    const entitySchema = changeLog.entitySchema as Selectable<IScheduleOfRates>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.schedule_name !== "string") {
      entitySchema.schedule_name = "";
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
