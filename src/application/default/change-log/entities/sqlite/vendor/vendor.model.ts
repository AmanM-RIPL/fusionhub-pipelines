import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../../common/types/entity";

export interface IVendor {
  id: Generated<number>;
  global_id: string;
  approval_status: boolean;
  change_history: JSONColumnType<ChangeHistory>;
  vendor_name: string;
  vendor_address: string | null;
  vendor_contact_person: string | null;
  vendor_mobile: string | null;
  vendor_email: string | null;
}