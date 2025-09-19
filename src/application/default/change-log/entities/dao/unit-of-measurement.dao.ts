import { IUnitOfMeasurement } from "../../entities/model/change-log.model";
import sqliteDb from "../../../../../infrastructure/db/dbsqlite/sqlite-db";

/**
 * UnitOfMeasurementDao
 */
export class UnitOfMeasurementDao {
  constructor(private readonly db = sqliteDb) {}

  /**
   * Insert or update entity in one call.
   */
  async upsert(entity: IUnitOfMeasurement): Promise<IUnitOfMeasurement> {
    const stmt = this.db.prepare(`
      INSERT INTO UnitOfMeasurement (
        global_id, approval_status, uom_name, unit_type,
        conversion_to_sqm, conversion_to_cubic_meter,
        conversion_to_meter, conversion_to_kilogram, change_history
      ) VALUES (
        @global_id, @approval_status, @uom_name, @unit_type,
        @conversion_to_sqm, @conversion_to_cubic_meter,
        @conversion_to_meter, @conversion_to_kilogram, @change_history
      )
      ON CONFLICT(global_id) DO UPDATE SET
        approval_status           = excluded.approval_status,
        uom_name                  = excluded.uom_name,
        unit_type                 = excluded.unit_type,
        conversion_to_sqm         = excluded.conversion_to_sqm,
        conversion_to_cubic_meter = excluded.conversion_to_cubic_meter,
        conversion_to_meter       = excluded.conversion_to_meter,
        conversion_to_kilogram    = excluded.conversion_to_kilogram,
        change_history            = excluded.change_history
      RETURNING *
    `);

    const row = stmt.get(entity);
    return row as IUnitOfMeasurement;
  }

  /**
   * Find a record by its numeric id or global_id (unique identifier).
   */
  async findById(id?: number, global_id?: string): Promise<IUnitOfMeasurement | undefined> {
    const row = this.db.prepare(`
      SELECT *
      FROM UnitOfMeasurement
      WHERE (@id IS NOT NULL AND id = @id)
         OR (@global_id IS NOT NULL AND global_id = @global_id)
      LIMIT 1
    `).get({ id, global_id });

    return row as IUnitOfMeasurement | undefined;
  }

  /**
   * Fetch all records from UnitOfMeasurement.
   */
  async findAll(): Promise<IUnitOfMeasurement[]> {
    const rows = this.db.prepare(`
      SELECT *
      FROM UnitOfMeasurement
    `).all();

    return rows as IUnitOfMeasurement[];
  }
}