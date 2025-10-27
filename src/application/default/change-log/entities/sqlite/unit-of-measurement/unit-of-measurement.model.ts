import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IUnitOfMeasurement {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  change_history: JSONColumnType<ChangeHistory>;
  uom_name: string;
  unit_type: string;
  conversion_to_sqm: number;
  conversion_to_cubic_meter: number;
  conversion_to_meter: number;
  conversion_to_kilogram: number;
}