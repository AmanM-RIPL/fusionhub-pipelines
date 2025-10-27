import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IMaterial {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  material_name: string;
  category: string;
  unit_of_measurement_id: number;
  change_history: JSONColumnType<ChangeHistory>;
}