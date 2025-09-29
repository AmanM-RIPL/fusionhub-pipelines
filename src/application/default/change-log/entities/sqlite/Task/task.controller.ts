import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { ITask } from "./task.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class TaskController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {
    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<ITask> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        task_name: "",
        description: "",
        bim_element: "",
        start_date: "",
        end_date: "",
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

    const entitySchema = changeLog.entitySchema as Selectable<ITask>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.task_name !== "string") {
      entitySchema.task_name = "";
    }

    if (typeof entitySchema.description !== "string") {
      entitySchema.description = "";
    }

    if (typeof entitySchema.bim_element !== "string") {
      entitySchema.bim_element = "";
    }

    if (typeof entitySchema.start_date !== "string") {
      entitySchema.start_date = "";
    }

    if (typeof entitySchema.end_date !== "string") {
      entitySchema.end_date = "";
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
