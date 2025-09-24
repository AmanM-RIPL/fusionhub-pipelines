import Database, { Database as DatabaseType } from "better-sqlite3";
import { IChangeLog } from "../../change-log.model";
import { Selectable } from "kysely";
import { IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import { getSQLiteDao } from "./index.factory";

export class SQLiteSync {
  constructor(protected readonly db: DatabaseType) {}

  public async syncSqlite(changeLogs: Selectable<IChangeLog>[]) {
    
    const sqliteTransactions = this.db.transaction(() => {
      for (const changeLog of changeLogs) {
        const dao: IProjectEntityBaseRepository | undefined = getSQLiteDao(changeLog.entity, this.db);

        if (!dao) {
          // entity is not in sqlite, skip
          continue;
        }

        if (changeLog.associatedApprovedEntity === null) {
          dao.create(changeLog);
        } else {
          dao.update(changeLog);
        }
      }
    });

    try {
      sqliteTransactions();
    } catch (error) {
      console.error("Error during SQLite sync transaction:", error);
      throw error;
    }
  }
}

/*
// Entity -> Table mappings
  private entityMappings: { [key: string]: ProjectEntityMapping } = {
    UnitOfMeasurement: { entityLocation: "sqlite", dao: UnitOfMeasurementDao },
    Vendor: { entityLocation: "sqlite", localFolderName: "vendor" },
    Material: { entityLocation: "sqlite", localFolderName: "material" },
    BudgetHead: { entityLocation: "sqlite", localFolderName: "budget-head" },
    ProjectBudget: { entityLocation: "sqlite", localFolderName: "project-budget" },
    ScheduleSetup: { entityLocation: "sqlite", localFolderName: "schedule-setup" },
    ScheduleOfRates: { entityLocation: "sqlite", localFolderName: "schedule-of-rates" },
    ScheduleOfRatesLine: { entityLocation: "sqlite", localFolderName: "schedule-of-rates-line" },
    Task: { entityLocation: "sqlite", localFolderName: "task" },
    TaskImage: { entityLocation: "sqlite", localFolderName: "task-image" },
    TaskMeasurement: { entityLocation: "sqlite", localFolderName: "task-measurement" },
    BillOfQuantity: { entityLocation: "sqlite", localFolderName: "bill-of-quantity" },
    BillOfQuantityLine: { entityLocation: "sqlite", localFolderName: "bill-of-quantity-line" },
    File: { entityLocation: "sqlite", localFolderName: "file" },
    FilePermission: { entityLocation: "sqlite", localFolderName: "file-permission" },
    PurchaseOrder: { entityLocation: "sqlite", localFolderName: "purchase-order" },
    PurchaseOrderLine: { entityLocation: "sqlite", localFolderName: "purchase-order-line" },
    GoodReceivedNote: { entityLocation: "sqlite", localFolderName: "good-received-note" },
    MaterialIndent: { entityLocation: "sqlite", localFolderName: "material-indent" },
    WorkOrder: { entityLocation: "sqlite", localFolderName: "work-order" },
    WorkOrderLine: { entityLocation: "sqlite", localFolderName: "work-order-line" },
    WorkBilling: { entityLocation: "sqlite", localFolderName: "work-billing" },
    WorkBillingLine: { entityLocation: "sqlite", localFolderName: "work-billing-line" },
  };


  public syncEntityArrayToSQLite(
    entities: any[]
  ): { inserted: number; updated: number; skipped: number } {
    let inserted = 0,
      updated = 0,
      skipped = 0;

    const transaction = this.db.transaction((items: any[]) => {
      for (const entityMappings of items) {
        const result = this.syncSingleEntity(entityMappings);
        if (result.action === "inserted") inserted++;
        else if (result.action === "updated") updated++;
        else skipped++;
      }
    });

    transaction(entities);
    return { inserted, updated, skipped };
  }
  private syncSingleEntity(entity: any): { action: string; table: string; id?: any } {
    const entityName = entity.entity;
    
        console.log("dd",entityName);
    const tableName = this.entityMappings[entityName];

    if (!tableName) {
      console.warn(`Unknown entity: ${entityName}`);
      return { action: "skipped", table: "unknown" };
    }

    // Extract data
    const entityData =  entity.entityData || entity;
    const columns = Object.keys(entityData).filter(
      (key) =>
        !["entityName", "tenant", "createdOn", "project" , "entity" ,"entitySchema", "associatedApprovedEntity","createdByUser"].includes(key)
    );

    if (columns.length === 0) {
      return { action: "skipped", table: tableName };
    }

    if (!columns.includes("id")) {
      throw new Error(` No primary key ("id") found for entity: ${entityName}`);
    }
    const primaryKey = "id";
    const primaryValue = entityData[primaryKey];
    if (!primaryValue) {
      console.warn(`No primary key for ${entityName}`);
      return { action: "skipped", table: tableName };
    }

    // Convert values
    const values = columns.map((col) => {
      const v = entityData[col];
      if (v === null || v === undefined) return null;
      if (typeof v === "object") return JSON.stringify(v);
      return String(v);
    });

    // Check existence
    const row = this.db.prepare(`SELECT 1 FROM ${tableName} WHERE ${primaryKey} = ?`).get(primaryValue);

    if (row) {
      // UPDATE
      const updateCols = columns.filter((c) => c !== primaryKey);
      const sql = `UPDATE ${tableName} 
                   SET ${updateCols.map((c) => `${c} = ?`).join(", ")}, updated_at = CURRENT_TIMESTAMP 
                   WHERE ${primaryKey} = ?`;
      const stmt = this.db.prepare(sql);
      stmt.run([...updateCols.map((c) => entityData[c]), primaryValue]);
      return { action: "updated", table: tableName, id: primaryValue };
    } else {
      // INSERT
      const sql = `INSERT INTO ${tableName} (${columns.join(", ")})
                   VALUES (${columns.map(() => "?").join(", ")})`;
      const stmt = this.db.prepare(sql);
      stmt.run(values);
      return { action: "inserted", table: tableName, id: primaryValue };
    }
  }
  */
