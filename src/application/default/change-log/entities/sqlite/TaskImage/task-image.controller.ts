 import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { ITaskImage } from "./task-image.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class TaskImageController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<ITaskImage> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        task_id: 0,
        image_url: "",
        image_local_path: "",
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

    const entitySchema = changeLog.entitySchema as Selectable<ITaskImage>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.task_id !== "number") {
      entitySchema.task_id = 0;
    }

    if (typeof entitySchema.image_url !== "string") {
      entitySchema.image_url = "";
    }

    if (typeof entitySchema.image_local_path !== "string") {
      entitySchema.image_local_path = "";
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
