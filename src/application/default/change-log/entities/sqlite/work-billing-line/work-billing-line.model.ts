import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IWorkBillingLine {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  work_order_line_id: number;
  dollar_value?: number;
  tax_amount?: number;
  tax_withholding_amount?: number;
  retention_amount?: number;
  change_history: JSONColumnType<ChangeHistory>;
}
