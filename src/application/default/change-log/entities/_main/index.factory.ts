import { Database as DatabaseType } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import UnitOfMeasurementDao from "../sqlite/unit-of-measurement/unit-of-measurement.dao";

// Factory to get DAO based on entity name
export function getSQLiteDao(entityName: string, db: DatabaseType): IProjectEntityBaseRepository | undefined {
  switch (entityName) {
    case "UnitOfMeasurement":
      return new UnitOfMeasurementDao(db);
    default:
      return undefined;
  }
}