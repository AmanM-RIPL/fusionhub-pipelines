import { Generated } from "kysely";

export interface IProject {
  id: Generated<number>;
  createdOn: Date;
  name: string;
  description: string | null;
  projectCode: string;
  startDate: Date;
  endDate: Date | null;
  status: string;
  contactEmail: string | null;
  contactMobile: string | null;
  tenantId: number | null;
  isBlocked: Generated<boolean>;
}

