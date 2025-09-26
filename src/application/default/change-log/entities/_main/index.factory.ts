import { Database as DatabaseType } from "better-sqlite3";
import { IProjectEntityBaseController, IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import UnitOfMeasurementDao from "../sqlite/unit-of-measurement/unit-of-measurement.dao";
import { UnitOfMeasurementController } from "../sqlite/unit-of-measurement/unit-of-measurement.controller";

// get DAO based on entity name
export function getSQLiteDao(entityName: string, db: DatabaseType): IProjectEntityBaseRepository | undefined {
  switch (entityName) {
    case "UnitOfMeasurement":
      return new UnitOfMeasurementDao(db);
    default:
      return undefined;
  }
}

// get Controller based on entity name
export function getSqLiteController(entityName: string) : IProjectEntityBaseController | undefined {
  switch (entityName) {
    case "UnitOfMeasurement":
      return new UnitOfMeasurementController();
    default:
      return undefined;
  }
}
