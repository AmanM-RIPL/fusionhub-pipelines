import { Database } from "better-sqlite3";
import { IProjectEntityBaseRepository } from "../../../../../common/repositories/project-entity.repository";
import { IChangeLog } from "../../../change-log.model";
import { Selectable } from "kysely";
import { IPurchaseOrderLine } from "./purchase-order-line.model";

export default class PurchaseOrderLineDao implements IProjectEntityBaseRepository {
  constructor(protected readonly db: Database) {}

  public create(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IPurchaseOrderLine = changeLog.entitySchema as IPurchaseOrderLine;

    const statement = this.db.prepare(`INSERT INTO PurchaseOrderLine ( global_id, approval_status, purchase_order_id, material_id, quantity, unit_of_measurement_id, dollar_value, tax_amount, tax_withholding, change_history) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)`);

    statement.run(
      changeLog.id,
      entitySchema.approval_status ? 1 : 0,
      entitySchema.purchase_order_id,
      entitySchema.material_id,
      entitySchema.quantity,
      entitySchema.unit_of_measurement_id,
      entitySchema.dollar_value,
      entitySchema.tax_amount,
      entitySchema.tax_withholding,
      JSON.stringify(entitySchema.change_history)
    );
  }

  public update(changeLog: Selectable<IChangeLog>): void {
    const entitySchema: IPurchaseOrderLine = changeLog.entitySchema as IPurchaseOrderLine;

    const statement = this.db.prepare(`UPDATE PurchaseOrderLine SET approval_status = ?, purchase_order_id = ?, material_id = ?, quantity = ?, unit_of_measurement_id = ?, dollar_value = ?, tax_amount = ?, tax_withholding = ?, change_history = ? WHERE global_id = ?`);

    statement.run(
      entitySchema.approval_status ? 1 : 0,
      entitySchema.purchase_order_id,
      entitySchema.material_id,
      entitySchema.quantity,
      entitySchema.unit_of_measurement_id,
      entitySchema.dollar_value,
      entitySchema.tax_amount,
      entitySchema.tax_withholding,
      JSON.stringify(entitySchema.change_history),
      changeLog.associatedApprovedEntity
    );
  }
}
