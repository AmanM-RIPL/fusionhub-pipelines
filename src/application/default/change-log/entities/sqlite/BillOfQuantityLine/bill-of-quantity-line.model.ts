import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IBillOfQuantityLine {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  bill_of_quantity_id: number;
  description: string;
  dollar_value: number;
  task_id: number;
  change_history: JSONColumnType<ChangeHistory>;
}
