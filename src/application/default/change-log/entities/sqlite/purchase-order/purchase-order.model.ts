import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IPurchaseOrder {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  vendor_id: number;
  change_history: JSONColumnType<ChangeHistory>;
}
