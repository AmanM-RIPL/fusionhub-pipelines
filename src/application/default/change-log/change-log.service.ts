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
  ) { }

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
    const pgProject = await this.projectRepository.findById(projectId);
    if (!pgProject) throw new Error(`Project ${projectId} not found in Postgres.`);

    let lastChangeLogId = pgProject.lastChangeLogId ?? 0;
    const batchSize = 10;
    let hasMore = true;

    // Open SQLite connection
    const dbFilePath = path.resolve("C:/code/fhapi_service/src/infrastructure/sqlite/" + "project_" + projectId + ".db");
    const sqliteDb = new Database(dbFilePath);

    try {
      const sqliteSync: SQLiteSync = new SQLiteSync(sqliteDb);

      // Process in batches using cursor
      while (hasMore) {
        const entitiesToSync = await this.changeLogRepository.findAllWithChangeLogGreaterThan(
          projectId,
          lastChangeLogId,
          batchSize
        );

        if (entitiesToSync.length === 0) {
          console.log(`No more change logs for project ${projectId}`);
          hasMore = false;
          break;
        }

        console.log(`Syncing batch of ${entitiesToSync.length} change logs for project ${projectId}`);

        // Sync current batch to SQLite
        await sqliteSync.syncSqlite(entitiesToSync);

        // Update cursor to the last processed ID
        lastChangeLogId = entitiesToSync[entitiesToSync.length - 1].id;

        // If fewer than batch size, process are done
        if (entitiesToSync.length < batchSize) {
          hasMore = false;
        }

        // This provides checkpoint capability in case of failures
        await this.projectRepository.update(projectId, {
          lastChangeLogId: lastChangeLogId
        });
      }

      console.log(`Completed sync for project ${projectId}. Last ID: ${lastChangeLogId}`);

    } catch (error) {
      console.error("Error during SQLite sync:", error);
      throw error;
    } finally {
      sqliteDb.close();
    }
  }

}
