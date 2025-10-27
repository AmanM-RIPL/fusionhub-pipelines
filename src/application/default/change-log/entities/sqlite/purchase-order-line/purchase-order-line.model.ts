import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IPurchaseOrderLine {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  purchase_order_id: number;
  material_id: number;
  quantity: number;
  unit_of_measurement_id: number;
  dollar_value: number;
  tax_amount: number;
  tax_withholding: number;
  change_history: JSONColumnType<ChangeHistory>;
}
