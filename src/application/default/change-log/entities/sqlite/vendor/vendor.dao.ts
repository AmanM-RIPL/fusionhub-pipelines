import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IVendor } from "./vendor.model";

export default class VendorDao implements IProjectEntityBaseRepository {

  constructor(protected readonly db: Database) { }

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IVendor = changeLog.entitySchema as IVendor;

    const statement = this.db.prepare(`
    INSERT INTO Vendor (
      global_id,
      approval_status,
      vendor_name,
      vendor_address,
      vendor_contact_person,
      vendor_mobile,
      vendor_email,
      change_history
    ) VALUES (?, ?, ?, ?, ?, ?, ?, ?)
  `);
console.log("ccc",statement);
    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.vendor_name ?? null,
      entitySchema.vendor_address ?? null,
      entitySchema.vendor_contact_person ?? null,
      entitySchema.vendor_mobile ?? null,
      entitySchema.vendor_email ?? null,
      JSON.stringify(entitySchema.change_history ?? null)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IVendor = changeLog.entitySchema as IVendor;
    const statement = this.db.prepare("UPDATE Vendor SET approval_status = ?, vendor_name = ?, vendor_address = ?, vendor_contact_person = ?, vendor_mobile = ?, vendor_email = ?, change_history = ? WHERE global_id = ?");

    statement.run(
      entitySchema.approval_status,
      entitySchema.vendor_name,
      entitySchema.vendor_address,
      entitySchema.vendor_contact_person,
      entitySchema.vendor_mobile,
      entitySchema.vendor_email,
      entitySchema.change_history,
      changeLog.associatedApprovedEntity
    );
  }
}