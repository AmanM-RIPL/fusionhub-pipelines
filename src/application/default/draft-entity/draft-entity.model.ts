import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory, ColumnValue } from "../../common/types/entity";

export interface IDraftEntity {
  id: Generated<number>;
  tenant: number;
  createdByUser: number;
  nextApprovingUser: number | null;
  entitySchema: string;
  associatedApprovedEntity: number | null;
  parentDraftEntity: number | null;
  changeHistory: JSONColumnType<ChangeHistory>;
  data: JSONColumnType<ColumnValue>;
};