import { Insertable, Selectable } from "kysely";
import { IProjectRepository } from "../project/project.repository";
import { IChangeLogRepository } from "./change-log.repository";
import { IChangeLog } from "./change-log.model";
import { IProject } from "../project/project.model";
import { InsertableEntity, UpdateableEntity } from "../../common/types/entity";
import sqliteDb from "../../../infrastructure/db/dbsqlite/sqlite-db";
import path from "path";
import fs from "fs";
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

  /*
    Added for just now. Will remove later. Change log need to automatically created in draftEntityService.approved
  */
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

  async ChangeLogSync(projectId: number): Promise<Selectable<IProject> | undefined> {
    const pgProject = await this.projectRepository.findById(projectId);

    if (!pgProject) {
      throw new Error(`Project ${projectId} not found in Postgres.`);
    }

    const lastChangeLogId = pgProject.lastChangeLogId ?? null;

    if (lastChangeLogId === null) {
      throw new Error(`Project ${projectId} has null lastChangeLogId`);
    }

    const entitiesToSync = await this.changeLogRepository.findAllWithChangeLogGreaterThan(
      projectId,
      lastChangeLogId,
      10,
      0
    );

    if (entitiesToSync.length === 0) {
      console.log(`No new entities found for project ${projectId}.`);
      return;
    }

    // Sync to SQLite
    for (const entity of entitiesToSync) {
      const existing = await this.changeLogRepository.findById(entity.id);

      if (!existing) {
        console.log(`Inserting new entity ${entity.id} into SQLite...`);
        await this.changeLogRepository.upsert(entity);
      }

      console.log(`Synced ${entitiesToSync.length} entities to SQLite for project ${projectId}.`);

      return pgProject;
    }
  }
}
