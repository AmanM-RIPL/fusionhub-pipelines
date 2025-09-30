import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IFilePermission {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  user_id: number;
  file_id: number;
  permission_type: string;
  change_history: JSONColumnType<ChangeHistory>;
}
