import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IGoodReceivedNote {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  purchase_order_line_id: number;
  amount_of_material_received: number;
  change_history: JSONColumnType<ChangeHistory>;
}
