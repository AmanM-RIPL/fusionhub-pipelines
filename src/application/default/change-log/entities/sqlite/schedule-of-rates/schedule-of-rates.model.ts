import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IScheduleOfRates {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  schedule_name: string;
  change_history: JSONColumnType<ChangeHistory>;
}
