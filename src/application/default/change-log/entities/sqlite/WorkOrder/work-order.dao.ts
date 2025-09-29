import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IWorkOrder } from "./work-order.model";

export default class WorkOrderDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkOrder = changeLog.entitySchema as IWorkOrder;

    const statement = this.db.prepare(`INSERT INTO WorkOrder ( global_id, approval_status, vendor_id, description, change_history ) VALUES (?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.vendor_id,
      entitySchema.description,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkOrder = changeLog.entitySchema as IWorkOrder;

    const statement = this.db.prepare(`UPDATE WorkOrder SET  approval_status = ?, vendor_id = ?, description = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.vendor_id,
      entitySchema.description,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
