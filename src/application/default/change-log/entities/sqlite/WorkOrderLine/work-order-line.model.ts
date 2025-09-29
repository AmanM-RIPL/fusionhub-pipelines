import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IWorkOrderLine {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  work_order_id: number;
  description: string;
  dollar_value: number;
  tax_amount: number;
  tax_withholding_amount: number;
  task_id: number | null;
  retention_amount: number;
  change_history: JSONColumnType<ChangeHistory>;
}
