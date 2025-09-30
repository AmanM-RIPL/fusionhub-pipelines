import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IGoodReceivedNote } from "./good-received-note.model";

export default class GoodReceivedNoteDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IGoodReceivedNote = changeLog.entitySchema as IGoodReceivedNote;

    const statement = this.db.prepare(`INSERT INTO GoodReceivedNote ( global_id, approval_status, purchase_order_line_id, amount_of_material_received, change_history) VALUES (?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.purchase_order_line_id,
      entitySchema.amount_of_material_received,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IGoodReceivedNote = changeLog.entitySchema as IGoodReceivedNote;

    const statement = this.db.prepare(`UPDATE GoodReceivedNote SET approval_status = ?, purchase_order_line_id = ?, amount_of_material_received = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.purchase_order_line_id,
      entitySchema.amount_of_material_received,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
