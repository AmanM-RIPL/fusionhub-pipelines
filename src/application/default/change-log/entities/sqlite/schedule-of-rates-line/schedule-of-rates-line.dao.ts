import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IScheduleOfRatesLine } from "./schedule-of-rates-line.model";

export default class ScheduleOfRatesLineDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleOfRatesLine = changeLog.entitySchema as IScheduleOfRatesLine;

    const statement = this.db.prepare(`INSERT INTO ScheduleOfRatesLine (global_id, approval_status, schedule_id, schedule_type_id, cost, resource, change_history) VALUES (?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_id,
      entitySchema.schedule_type_id,
      entitySchema.cost,
      entitySchema.resource,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleOfRatesLine = changeLog.entitySchema as IScheduleOfRatesLine;

    const statement = this.db.prepare(`UPDATE ScheduleOfRatesLine SET approval_status = ?, schedule_id = ?, schedule_type_id = ?, cost = ?, resource = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_id,
      entitySchema.schedule_type_id,
      entitySchema.cost,
      entitySchema.resource,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
