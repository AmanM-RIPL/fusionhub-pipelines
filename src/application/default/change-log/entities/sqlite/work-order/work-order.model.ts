import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IWorkOrder {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  vendor_id: number;
  description: string;
  change_history: JSONColumnType<ChangeHistory>;
}
