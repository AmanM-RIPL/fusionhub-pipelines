import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface ITaskImage {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  task_id: number;
  image_url: string;
  image_local_path: string;
  change_history: JSONColumnType<ChangeHistory>;
}
