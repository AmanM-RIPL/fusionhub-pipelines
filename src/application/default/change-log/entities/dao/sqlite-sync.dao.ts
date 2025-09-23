import Database, { Database as DatabaseType } from "better-sqlite3";
import path from "path";

export class SQLiteSyncDao {
   private db: DatabaseType;

  // Entity -> Table mappings
  private entityMappings: { [key: string]: string } = {
    UnitOfMeasurement: "UnitOfMeasurement",
    Vendor: "Vendor",
    Material: "Material",
    BudgetHead: "BudgetHead",
    ProjectBudget: "ProjectBudget",
    ScheduleSetup: "ScheduleSetup",
    ScheduleOfRates: "ScheduleOfRates",
    ScheduleOfRatesLine: "ScheduleOfRatesLine",
    Task: "Task",
    TaskImage: "TaskImage",
    TaskMeasurement: "TaskMeasurement",
    BillOfQuantity: "BillOfQuantity",
    BillOfQuantityLine: "BillOfQuantityLine",
    File: "File",
    FilePermission: "FilePermission",
    PurchaseOrder: "PurchaseOrder",
    PurchaseOrderLine: "PurchaseOrderLine",
    GoodReceivedNote: "GoodReceivedNote",
    MaterialIndent: "MaterialIndent",
    WorkOrder: "WorkOrder",
    WorkOrderLine: "WorkOrderLine",
    WorkBilling: "WorkBilling",
    WorkBillingLine: "WorkBillingLine",
  };

  constructor(dbFile: string = "C:/code/fhapi_service/src/infrastructure/sqlite/project_19.db") {
    const fullDbPath = path.resolve(dbFile);
    this.db = new Database(fullDbPath);
    console.log(`SQLite connected: ${fullDbPath}`);
  }

  /**
   * Sync array of entities
   */
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

  /**
   * Sync single entity (insert or update)
   */
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
}
