import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { ITaskImage } from "./task-image.model";

export default class TaskImageDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITaskImage = changeLog.entitySchema as ITaskImage;

    const statement = this.db.prepare(`INSERT INTO TaskImage ( global_id, approval_status, task_id, image_url, image_local_path, change_history ) VALUES (?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_id,
      entitySchema.image_url,
      entitySchema.image_local_path,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITaskImage = changeLog.entitySchema as ITaskImage;

    const statement = this.db.prepare(` UPDATE TaskImage SET approval_status = ?, task_id = ?, image_url = ?, image_local_path = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_id,
      entitySchema.image_url,
      entitySchema.image_local_path,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
