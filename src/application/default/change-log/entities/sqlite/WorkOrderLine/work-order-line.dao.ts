import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IWorkOrderLine } from "./work-order-line.model";

export default class WorkOrderLineDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkOrderLine = changeLog.entitySchema as IWorkOrderLine;

    const statement = this.db.prepare(`INSERT INTO WorkOrderLine ( global_id, approval_status, work_order_id, description, dollar_value, tax_amount, tax_withholding_amount, task_id,  retention_amount, change_history ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.work_order_id,
      entitySchema.description,
      entitySchema.dollar_value,
      entitySchema.tax_amount,
      entitySchema.tax_withholding_amount,
      entitySchema.task_id ?? null,
      entitySchema.retention_amount,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkOrderLine = changeLog.entitySchema as IWorkOrderLine;

    const statement = this.db.prepare(`UPDATE WorkOrderLine SET approval_status = ?, work_order_id = ?, description = ?, dollar_value = ?, tax_amount = ?, tax_withholding_amount = ?, task_id = ?, retention_amount = ?,  change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.work_order_id,
      entitySchema.description,
      entitySchema.dollar_value,
      entitySchema.tax_amount,
      entitySchema.tax_withholding_amount,
      entitySchema.task_id ?? null,
      entitySchema.retention_amount,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
