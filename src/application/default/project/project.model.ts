import { Generated } from "kysely";

export interface IProject {
  id: Generated<number>;
  tenant: number;
  name: string;
  description: string | null;
  projectCode: string;
  startDate: Date | null;
  endDate: Date | null;
  status: string;
  contactEmail: string | null;
  contactMobile: string | null;
  createdOn: Generated<Date>;
  isBlocked: Generated<boolean>;
}


