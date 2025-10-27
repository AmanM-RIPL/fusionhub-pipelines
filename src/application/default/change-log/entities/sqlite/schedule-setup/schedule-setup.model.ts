import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IScheduleSetup {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  schedule_name: string;
  description: string;
  cost_parameter: string;
  resource_parameter: string;
  change_history: JSONColumnType<ChangeHistory>;
}
