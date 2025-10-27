import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IUnitOfMeasurement } from "./unit-of-measurement.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";

export class UnitOfMeasurementController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IUnitOfMeasurement> = {
        uom_name: "",
        id: 0,
        global_id: 0,
        approval_status: false,
        change_history: {
          user: 0,
          changeType: "create",
          description: "",
          timestamp: new Date(),
          approvalHistory: []
        },
        unit_type: "",
        conversion_to_sqm: 0,
        conversion_to_cubic_meter: 0,
        conversion_to_meter: 0,
        conversion_to_kilogram: 0
      };
    }
    
    const entitySchema = changeLog.entitySchema as Selectable<IUnitOfMeasurement>;

    if (typeof entitySchema.uom_name !== "string") {
      entitySchema.uom_name = "";
    }

    if (typeof entitySchema.unit_type !== "string") {
      entitySchema.unit_type = "";
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = false;
    }

    if (typeof entitySchema.conversion_to_sqm !== "number") {
      entitySchema.conversion_to_sqm = 0;
    }

    if (typeof entitySchema.conversion_to_cubic_meter !== "number") {
      entitySchema.conversion_to_cubic_meter = 0;
    }

    if (typeof entitySchema.conversion_to_meter !== "number") {
      entitySchema.conversion_to_meter = 0;
    }

    if (typeof entitySchema.conversion_to_kilogram !== "number") {
      entitySchema.conversion_to_kilogram = 0;
    }
    changeLog.entitySchema = entitySchema
  }

}