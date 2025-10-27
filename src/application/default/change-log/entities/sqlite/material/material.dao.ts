import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IMaterial } from "./material.model";

export default class MaterialDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) { }

  public create(changeLog: Selectable<IChangeLog>): void {
  const entitySchema: IMaterial = changeLog.entitySchema as IMaterial;

  const sql = ` INSERT INTO Material ( global_id, approval_status, material_name, category, unit_of_measurement_id, change_history) VALUES (?, ?, ?, ?, ?, ?)`;

  const values = [ changeLog.id, entitySchema.approval_status ? 1 : 0, entitySchema.material_name, entitySchema.category, entitySchema.unit_of_measurement_id, JSON.stringify(changeLog.changeHistory)];

  const statement = this.db.prepare(sql);
  statement.run(...values);
}


  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IMaterial = changeLog.entitySchema as IMaterial;

    const statement = this.db.prepare(`UPDATE Material SET approval_status = ?, material_name = ?, category = ?, unit_of_measurement_id = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.material_name,
      entitySchema.category,
      entitySchema.unit_of_measurement_id,
      JSON.stringify(changeLog.changeHistory),
      changeLog.associatedApprovedEntity
    );
  }
}
