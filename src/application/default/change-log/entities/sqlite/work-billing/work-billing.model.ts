import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IWorkBilling {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  work_order_id: number;
  change_history: JSONColumnType<ChangeHistory>;
}
