import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory, ColumnValue } from "../../common/types/entity";

export interface IChangeLog {
  id: Generated<number>;
  tenant: number;
  createdOn:  Generated<Date>;
  project: number;
  entity: string;
  createdByUser: number;
  entitySchema: JSONColumnType<any>;
  associatedApprovedEntity: number | null;
  changeHistory: JSONColumnType<ChangeHistory>;
}
