import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IScheduleOfRatesLine {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  schedule_id: number;
  schedule_type_id: number;
  cost: string;
  resource: string;
  change_history: JSONColumnType<ChangeHistory>;
}
