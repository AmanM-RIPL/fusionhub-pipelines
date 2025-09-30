import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IBillOfQuantityLine } from "./bill-of-quantity-line.model";

export default class BillOfQuantityLineDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBillOfQuantityLine = changeLog.entitySchema as IBillOfQuantityLine;

    const statement = this.db.prepare(`INSERT INTO BillOfQuantityLine ( global_id, approval_status, bill_of_quantity_id, description, dollar_value, task_id, change_history) VALUES (?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.bill_of_quantity_id,
      entitySchema.description,
      entitySchema.dollar_value,
      entitySchema.task_id,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBillOfQuantityLine = changeLog.entitySchema as IBillOfQuantityLine;

    const statement = this.db.prepare(`UPDATE BillOfQuantityLine SET  approval_status = ?, bill_of_quantity_id = ?, description = ?, dollar_value = ?, task_id = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.bill_of_quantity_id,
      entitySchema.description,
      entitySchema.dollar_value,
      entitySchema.task_id,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
