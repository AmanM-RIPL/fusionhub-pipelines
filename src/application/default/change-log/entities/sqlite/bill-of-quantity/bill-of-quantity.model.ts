import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IBillOfQuantity {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  schedule_id: number;
  description: string;
  change_history: JSONColumnType<ChangeHistory>;
}
