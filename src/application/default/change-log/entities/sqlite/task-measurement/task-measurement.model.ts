import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface ITaskMeasurement {
  id: Generated<number>;
  global_id: number;
  approval_status: boolean;
  task_id: number;
  date: string;
  length: number;
  length_unit_id: number;
  width: number;
  width_unit_id: number;
  height: number;
  height_unit_id: number;
  diameter: number;
  diameter_unit_id: number;
  volume: number;
  volume_unit_id: number;
  surface_area: number;
  surface_area_unit_id: number;
  change_history: JSONColumnType<ChangeHistory>;
}
