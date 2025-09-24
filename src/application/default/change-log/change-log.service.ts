import { Insertable, Selectable } from "kysely";
import { IProjectRepository } from "../project/project.repository";
import { IChangeLogRepository } from "./change-log.repository";
import { IChangeLog } from "./change-log.model";
import { IProject } from "../project/project.model";
import { InsertableEntity } from "../../common/types/entity";
import { SQLiteSync } from "./entities/_main/sqlite.factory";
import path from "path";
import Database from "better-sqlite3";

export class ChangeLogService {
  constructor(
    private readonly changeLogRepository: IChangeLogRepository,
    private readonly projectRepository: IProjectRepository,
    private readonly user: number
  ) {}

  async findById(id: number): Promise<Selectable<IChangeLog> | undefined> {
    return await this.changeLogRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IChangeLog>[]> {
    return await this.changeLogRepository.findAll(limit, offset);
  }

  async create(entity: Omit<InsertableEntity<IChangeLog>, "createdByUser" | "changeHistory">): Promise<Selectable<IChangeLog>> {
    const entityToCreate: InsertableEntity<IChangeLog> = {
      ...entity,
      createdByUser: this.user,
      changeHistory: JSON.stringify({
        user: this.user,
        changeType: "create",
        description: "Approved By User",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: this.user,
            timestamp: new Date(),
            description: "Approved By User",
            status: "approved"
          }
        ]
      })
    };

    return await this.changeLogRepository.create(entityToCreate);
  }

  /**
   * Sync change logs from Postgres to SQLite
   */
   async changeLogSync(projectId: number): Promise<void> {
    // 1. Get project from Postgres
    const pgProject = await this.projectRepository.findById(projectId);
    if (!pgProject) throw new Error(`Project ${projectId} not found in Postgres.`);

    const lastChangeLogId = pgProject.lastChangeLogId ?? 0;

    // 2. Fetch new change logs from Postgres
    const entitiesToSync = await this.changeLogRepository.findAllWithChangeLogGreaterThan(
      projectId,
      lastChangeLogId,
      50,
      0
    );

    if (entitiesToSync.length === 0) {
      console.log(`No new change logs for project ${projectId}`);
      return;
    }

    // 3. Sync into SQLite
    console.log(`Starting sync of ${entitiesToSync.length} entities for project ${projectId}`);
    const dbFilePath = path.resolve("C:/code/fhapi_service/src/infrastructure/sqlite/" + "project_" + projectId + ".db");
    const sqliteDb = new Database(dbFilePath);
    
    try {
      const sqliteSync: SQLiteSync = new SQLiteSync(sqliteDb);
      await sqliteSync.syncSqlite(entitiesToSync);
    } catch (error) {
      console.error("Error during SQLite sync:", error);
      sqliteDb.close();
      throw error;
    }

    // close sqlite database
    sqliteDb.close();
    
    // console.log(`   Sync completed for project ${projectId}:`);
    // console.log(`   Inserted: ${syncResults.inserted}`);
    // console.log(`   Updated:  ${syncResults.updated}`);
    // console.log(`   Skipped:  ${syncResults.skipped}`);
  }

}
