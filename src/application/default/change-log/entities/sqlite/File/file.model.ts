import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IFile {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  description: string;
  file_url: string;
  file_local_path: string;
  change_history: JSONColumnType<ChangeHistory>;
}
