import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IFilePermission } from "./file-permission.model";

export default class FilePermissionDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IFilePermission = changeLog.entitySchema as IFilePermission;

    const statement = this.db.prepare(`INSERT INTO FilePermission ( global_id, approval_status, user_id, file_id, permission_type, change_history) VALUES (?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.user_id,
      entitySchema.file_id,
      entitySchema.permission_type,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IFilePermission = changeLog.entitySchema as IFilePermission;

    const statement = this.db.prepare(` UPDATE FilePermission SET approval_status = ?, user_id = ?, file_id = ?, permission_type = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.user_id,
      entitySchema.file_id,
      entitySchema.permission_type,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
