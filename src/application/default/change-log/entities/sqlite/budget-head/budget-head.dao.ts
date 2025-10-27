import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IBudgetHead } from "./budget-head.model";

export default class BudgetHeadDao implements IProjectEntityBaseRepository {

  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBudgetHead = changeLog.entitySchema as IBudgetHead;

    const statement = this.db.prepare(`INSERT INTO BudgetHead (global_id, approval_status, description, change_history) VALUES (?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.description,
      JSON.stringify(changeLog.changeHistory)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IBudgetHead = changeLog.entitySchema as IBudgetHead;

    const statement = this.db.prepare(`UPDATE BudgetHead SET approval_status = ?, description = ?, change_history = ? WHERE global_id = ?
    `);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.description,
      JSON.stringify(changeLog.changeHistory),
      changeLog.associatedApprovedEntity
    );
  }
}
