import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IPurchaseOrder } from "./purchase-order.model";

export default class PurchaseOrderDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IPurchaseOrder = changeLog.entitySchema as IPurchaseOrder;

    const statement = this.db.prepare(`
      INSERT INTO PurchaseOrder (global_id, approval_status, vendor_id, change_history ) VALUES (?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.vendor_id,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IPurchaseOrder = changeLog.entitySchema as IPurchaseOrder;

    const statement = this.db.prepare(`UPDATE PurchaseOrder SET approval_status = ?, vendor_id = ?, change_history = ? WHERE global_id = ?
    `);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.vendor_id,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
