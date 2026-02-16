import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface ITask {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  task_name: string;
  description: string;
  bim_element: string;
  start_date: string;
  end_date: string;
  change_history: JSONColumnType<ChangeHistory>;
}
