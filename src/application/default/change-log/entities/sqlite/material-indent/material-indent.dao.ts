import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IMaterialIndent } from "./material-indent.model";

export default class MaterialIndentDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IMaterialIndent = changeLog.entitySchema as IMaterialIndent;

    const statement = this.db.prepare(`INSERT INTO MaterialIndent (  global_id, approval_status, material_id, total_quantity, task_id, change_history ) VALUES (?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.material_id,
      entitySchema.total_quantity,
      entitySchema.task_id ?? null,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IMaterialIndent = changeLog.entitySchema as IMaterialIndent;

    const statement = this.db.prepare(` UPDATE MaterialIndent SET approval_status = ?, material_id = ?, total_quantity = ?, task_id = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.material_id,
      entitySchema.total_quantity,
      entitySchema.task_id ?? null,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
