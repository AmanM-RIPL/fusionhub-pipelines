import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IProjectBudget } from "./project-budget.model";

export default class ProjectBudgetDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IProjectBudget = changeLog.entitySchema as IProjectBudget;

    const statement = this.db.prepare(`INSERT INTO ProjectBudget ( global_id, approval_status, budget_head_id, dollar_value, change_history ) VALUES (?, ?, ?, ?, ?)
    `);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.budget_head_id,
      entitySchema.dollar_value,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IProjectBudget = changeLog.entitySchema as IProjectBudget;

    const statement = this.db.prepare(`UPDATE ProjectBudget SET approval_status = ?, budget_head_id = ?, dollar_value = ?, change_history = ? WHERE global_id = ? `);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.budget_head_id,
      entitySchema.dollar_value,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
