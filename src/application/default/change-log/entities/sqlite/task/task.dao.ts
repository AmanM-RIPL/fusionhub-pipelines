import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { ITask } from "./task.model";

export default class TaskDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITask = changeLog.entitySchema as ITask;

    const statement = this.db.prepare(`INSERT INTO Task ( global_id, approval_status, task_name, description, bim_element, start_date, end_date, change_history ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_name,
      entitySchema.description,
      entitySchema.bim_element,
      entitySchema.start_date,
      entitySchema.end_date,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITask = changeLog.entitySchema as ITask;

    const statement = this.db.prepare(` UPDATE Task SET approval_status = ?, task_name = ?, description = ?, bim_element = ?, start_date = ?, end_date = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_name,
      entitySchema.description,
      entitySchema.bim_element,
      entitySchema.start_date,
      entitySchema.end_date,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
