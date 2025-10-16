import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IWorkBillingLine } from "./work-billing-line.model";

export default class WorkBillingLineDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkBillingLine = changeLog.entitySchema as IWorkBillingLine;

    const statement = this.db.prepare(`INSERT INTO WorkBillingLine ( global_id, approval_status, work_order_id, change_history) VALUES (?, ?, ?, ?)`);

      statement.run(
          changeLog.id,
          entitySchema.approval_status ? 1 : 0,
          entitySchema.work_order_line_id,
          entitySchema.dollar_value,
          entitySchema.tax_amount,
          entitySchema.tax_withholding_amount,
          entitySchema.retention_amount,
          JSON.stringify(entitySchema.change_history)
      );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IWorkBillingLine = changeLog.entitySchema as IWorkBillingLine;

    const statement = this.db.prepare(` UPDATE WorkBillingLine SET approval_status = ?, work_order_line_id = ?, dollar_value= ?, tax_amount=?, tax_withholding_amount=?, retention_amount=?, change_history = ? WHERE global_id = ?`);

      statement.run(
          entitySchema.approval_status ? 1 : 0,
          entitySchema.work_order_line_id,
          entitySchema.dollar_value,
          entitySchema.tax_amount,
          entitySchema.tax_withholding_amount,
          entitySchema.retention_amount,
          JSON.stringify(entitySchema.change_history),
          changeLog.associatedApprovedEntity
      );
  }
}
