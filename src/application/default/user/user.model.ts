import { Generated, JSONColumnType } from "kysely";

export interface IUser {
  id: Generated<number>;
  tenant?: number;
  createdOn: Generated<Date>;
  username: string;
  password: string;
  email: string;
  firstName: string;
  lastName: string;
  mobile: string;
}