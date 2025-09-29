import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IScheduleOfRates } from "./schedule-of-rates.model";

export default class ScheduleOfRatesDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleOfRates = changeLog.entitySchema as IScheduleOfRates;

    const statement = this.db.prepare(`INSERT INTO ScheduleOfRates (global_id, approval_status, schedule_name, change_history) VALUES (?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_name,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IScheduleOfRates = changeLog.entitySchema as IScheduleOfRates;

    const statement = this.db.prepare(`UPDATE ScheduleOfRates SET approval_status = ?, schedule_name = ?, change_history = ? WHERE global_id = ? `);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.schedule_name,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
