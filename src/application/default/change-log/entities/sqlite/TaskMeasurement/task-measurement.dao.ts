import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { ITaskMeasurement } from "./task-measurement.model";

export default class TaskMeasurementDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITaskMeasurement = changeLog.entitySchema as ITaskMeasurement;

    const statement = this.db.prepare(`INSERT INTO TaskMeasurement ( global_id, approval_status, task_id, date, length, length_unit_id, width, width_unit_id, height, height_unit_id, diameter, diameter_unit_id, volume, volume_unit_id, surface_area, surface_area_unit_id, change_history) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_id,
      entitySchema.date,
      entitySchema.length,
      entitySchema.length_unit_id,
      entitySchema.width,
      entitySchema.width_unit_id,
      entitySchema.height,
      entitySchema.height_unit_id,
      entitySchema.diameter,
      entitySchema.diameter_unit_id,
      entitySchema.volume,
      entitySchema.volume_unit_id,
      entitySchema.surface_area,
      entitySchema.surface_area_unit_id,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: ITaskMeasurement = changeLog.entitySchema as ITaskMeasurement;

    const statement = this.db.prepare(`
      UPDATE TaskMeasurement SET approval_status = ?, task_id = ?, date = ?, length = ?, length_unit_id = ?, width = ?, width_unit_id = ?, height = ?, height_unit_id = ?, diameter = ?, diameter_unit_id = ?, volume = ?, volume_unit_id = ?, surface_area = ?, surface_area_unit_id = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.task_id,
      entitySchema.date,
      entitySchema.length,
      entitySchema.length_unit_id,
      entitySchema.width,
      entitySchema.width_unit_id,
      entitySchema.height,
      entitySchema.height_unit_id,
      entitySchema.diameter,
      entitySchema.diameter_unit_id,
      entitySchema.volume,
      entitySchema.volume_unit_id,
      entitySchema.surface_area,
      entitySchema.surface_area_unit_id,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
