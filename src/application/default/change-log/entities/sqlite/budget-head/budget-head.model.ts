import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IBudgetHead {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  description: string;
  change_history: JSONColumnType<ChangeHistory>;
}