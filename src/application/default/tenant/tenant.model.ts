import { Generated } from "kysely";

export interface ITenant {
  id: Generated<number>;
  createdOn: Generated<Date>;
  name: string;
  defaultEmail: string;
  defaultEmail1: string | null;
  defaultEmail2: string | null;
  defaultMobile: string;
  defaultMobile1: string | null;
  isBlocked: Generated<boolean>;
}