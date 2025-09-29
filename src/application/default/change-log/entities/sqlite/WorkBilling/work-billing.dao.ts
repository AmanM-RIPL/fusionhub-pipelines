import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IWorkBilling } from "./work-billing.model";

export default class WorkBillingDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkBilling = changeLog.entitySchema as IWorkBilling;

    const statement = this.db.prepare(`INSERT INTO WorkBilling ( global_id, approval_status, work_order_id, change_history) VALUES (?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.work_order_id,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkBilling = changeLog.entitySchema as IWorkBilling;

    const statement = this.db.prepare(` UPDATE WorkBilling SET approval_status = ?, work_order_id = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.work_order_id,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
