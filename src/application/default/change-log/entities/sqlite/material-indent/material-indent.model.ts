import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IMaterialIndent {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  material_id: number;
  total_quantity: number;
  task_id?: number | null;
  change_history: JSONColumnType<ChangeHistory>;
}
