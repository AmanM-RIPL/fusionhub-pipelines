import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IScheduleSetup } from "./schedule-setup.model";

export default class ScheduleSetupDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleSetup = changeLog.entitySchema as IScheduleSetup;

    const statement = this.db.prepare(`
      INSERT INTO ScheduleSetup (global_id, approval_status, schedule_name, description, cost_parameter, resource_parameter, change_history) VALUES (?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_name,
      entitySchema.description,
      entitySchema.cost_parameter,
      entitySchema.resource_parameter,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleSetup = changeLog.entitySchema as IScheduleSetup;

    const statement = this.db.prepare(`UPDATE ScheduleSetup SET approval_status = ?, schedule_name = ?, description = ?, cost_parameter = ?, resource_parameter = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_name,
      entitySchema.description,
      entitySchema.cost_parameter,
      entitySchema.resource_parameter,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
