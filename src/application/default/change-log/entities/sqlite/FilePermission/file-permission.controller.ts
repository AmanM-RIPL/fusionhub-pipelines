import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IFilePermission } from "./file-permission.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class FilePermissionController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IFilePermission> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        user_id: 0,
        file_id: 0,
        permission_type: "",
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

    const entitySchema = changeLog.entitySchema as Selectable<IFilePermission>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.user_id !== "number") {
      entitySchema.user_id = 0;
    }

    if (typeof entitySchema.file_id !== "number") {
      entitySchema.file_id = 0;
    }

    if (typeof entitySchema.permission_type !== "string") {
      entitySchema.permission_type = "";
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
