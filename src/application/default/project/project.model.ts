import { Generated } from "kysely";

export interface IProject {
  id: Generated<number>;
  tenant: number;
  projectName: string;
  description: string | null;
  startDate: Date | null;
  endDate: Date | null;
  status: string;
  customerName: string;
  customerEmail: string | null;
  customerMobile: string | null;
  lastChangeLogId:number | null,
  createdOn: Generated<Date>;
  isBlocked: Generated<boolean>;
}


