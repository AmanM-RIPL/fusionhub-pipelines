import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory, ColumnValue } from "../../common/types/entity";

export interface IDraftEntity {
  id: Generated<number>;
  tenant: number;
  createdOn:  Generated<Date>;
  project: number;
  entity: string;
  createdByUser: number;
  nextApprovingUser: number | null;
  entitySchema: JSONColumnType<any>;
  associatedApprovedEntity: number | null;
  changeHistory: JSONColumnType<ChangeHistory>;
}
