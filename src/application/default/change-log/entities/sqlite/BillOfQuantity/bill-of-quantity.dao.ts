import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IBillOfQuantity } from "./bill-of-quantity.model";

export default class BillOfQuantityDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBillOfQuantity = changeLog.entitySchema as IBillOfQuantity;

    const statement = this.db.prepare(`INSERT INTO BillOfQuantity (  global_id, approval_status, schedule_id, description, change_history) VALUES (?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_id,
      entitySchema.description,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBillOfQuantity = changeLog.entitySchema as IBillOfQuantity;

    const statement = this.db.prepare(` UPDATE BillOfQuantity SET approval_status = ?, schedule_id = ?, description = ?, change_history = ? WHERE global_id = ? `);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_id,
      entitySchema.description,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
