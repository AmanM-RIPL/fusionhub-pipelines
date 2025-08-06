import { Generated, JSONColumnType } from "kysely";
import { AccessPermission, FilterPermission, IColumnSchema } from "../../common/types/entity";

export interface IPermission {
  id: Generated<number>;
  tenant: number;
  createdOn: Generated<Date>;
  user: number;
  project: number;
  entity: string;
  permissionType: 'CREATE' | 'READ' | 'UPDATE';
  approval: number[];
  filter: JSONColumnType<FilterPermission>;
  access?: JSONColumnType<AccessPermission>;
}