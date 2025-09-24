import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IUnitOfMeasurement } from "./unit-of-measurement.model";

export default class UnitOfMeasurementDao implements IProjectEntityBaseRepository {

  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IUnitOfMeasurement = changeLog.entitySchema as IUnitOfMeasurement;

    const statement = this.db.prepare("INSERT INTO UnitOfMeasurement (uom_name, unit_type, conversion_to_sqm, conversion_to_cubic_meter, conversion_to_meter, conversion_to_kilogram, changeHistory) VALUES (?, ?, ?, ?, ?, ?, ?)");

    statement.run(
      entitySchema.uom_name,
      entitySchema.unit_type,
      entitySchema.conversion_to_sqm,
      entitySchema.conversion_to_cubic_meter,
      entitySchema.conversion_to_meter,
      entitySchema.conversion_to_kilogram,
      entitySchema.change_history
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IUnitOfMeasurement = changeLog.entitySchema as IUnitOfMeasurement;
    const statement = this.db.prepare("UPDATE UnitOfMeasurement SET uom_name = ?, unit_type = ?, conversion_to_sqm = ?, conversion_to_cubic_meter = ?, conversion_to_meter = ?, conversion_to_kilogram = ?, changeHistory = ? WHERE id = ?");

    statement.run(
      entitySchema.uom_name,
      entitySchema.unit_type,
      entitySchema.conversion_to_sqm,
      entitySchema.conversion_to_cubic_meter,
      entitySchema.conversion_to_meter,
      entitySchema.conversion_to_kilogram,
      entitySchema.change_history,
      changeLog.associatedApprovedEntity
    );
  }
}