import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IProjectBudget {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  budget_head_id: number;
  dollar_value: number;
  change_history: JSONColumnType<ChangeHistory>;
}
