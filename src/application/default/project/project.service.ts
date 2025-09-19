import { Insertable, Selectable, Transaction } from "kysely";
import { IProject } from "./project.model";
import { UpdateableEntity } from "../../common/types/entity";
import { IProjectRepository } from "../project/project.repository";
import { sqliteTable } from "../../../infrastructure/db/dbsqlite/sqliteTables"
import path from "path";
import fs from "fs";
import Database from "better-sqlite3";

export class ProjectService {
  constructor(protected readonly projectRepository: IProjectRepository) { }

  async findById(id: number): Promise<Selectable<IProject> | undefined> {
    return await this.projectRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IProject>[]> {
    return await this.projectRepository.findAll(limit, offset);
  }

  async create(projectDetails: Insertable<IProject>): Promise<Selectable<IProject>> {
    const __dirname = 'C:/code/fhapi_service/src/infrastructure/sqlite/projects/';
    const project = await this.projectRepository.create(projectDetails);
    const dbPath = path.join(__dirname, `project_${project.id}.db`);
    fs.mkdirSync(path.dirname(dbPath), { recursive: true });

    const sqliteDb = new Database(dbPath);

    try {
      sqliteDb.pragma("foreign_keys = ON");
      // Run all CREATE TABLE statements
      sqliteDb.transaction(() => {
        for (const sql of sqliteTable) {
          sqliteDb.prepare(sql).run();
        }
      })();
    } catch (error: any) {
      console.error("Database creation failed:", error.message);
      // If something failed, delete the incomplete database file
      if (fs.existsSync(dbPath)) {
        fs.unlinkSync(dbPath);
        console.log(`Deleted failed database file: ${dbPath}`);
      }
      throw error;
    } finally {
      if (sqliteDb) sqliteDb.close();
    }
    return project;
  }

  async update(id: number, updatedProject: UpdateableEntity<IProject>): Promise<Selectable<IProject> | undefined> {
    return await this.projectRepository.update(id, updatedProject);
  }

  async syncProject(projectId: number): Promise<void> {
    const pgProject = await this.projectRepository.findById(projectId);

    if (!pgProject) {
      throw new Error(`Project ${projectId} not found in Postgres.`);
    }
    const __dirname = "C:/code/fhapi_service/src/infrastructure/sqlite/projects/";
    const sqliteDbPath = path.join(__dirname, `project_${projectId}.db`);

    if (!fs.existsSync(sqliteDbPath)) {
      throw new Error(`SQLite DB for project ${projectId} not found.`);
    }

    const sqliteDb = new Database(sqliteDbPath);

    // Get last synced ChangeLogId from SQLite
    interface ProjectRow {
      lastChangeLogId: number | null;
    }
    const sqliteProject = sqliteDb
      .prepare<[number], ProjectRow>(
        `SELECT lastChangeLogId FROM project WHERE id = ?`
      )
      .get(projectId);

    const sqliteLastId = sqliteProject?.lastChangeLogId ?? null;
    if (sqliteLastId === null) {
      throw new Error(`Project ${sqliteLastId} is null or undefined.`);
    }
    const pgLastId = pgProject.lastChangeLogId ?? 0;

    //sync newer changes
    if (pgLastId > sqliteLastId) {
      const entitiesToSync: Selectable<IProject>[] =
        await this.projectRepository.findAllWithChangeLogGreaterThan(
          projectId,
          sqliteLastId,
          0,
          100
        );

      if (entitiesToSync.length === 0) {
        console.log(`No new entities found for project ${projectId}.`);
        return;
      }

      // Insert/update entities in SQLite
      const stmt = sqliteDb.prepare<Insertable<IProject>>(`
        INSERT INTO project (
          id, tenant, projectName, description, startDate, endDate,
          status, customerName, customerEmail, customerMobile,
          lastChangeLogId, createdOn, isBlocked
        ) VALUES (
          @id, @tenant, @projectName, @description, @startDate, @endDate,
          @status, @customerName, @customerEmail, @customerMobile,
          @lastChangeLogId, @createdOn, @isBlocked
        )
        ON CONFLICT(id) DO UPDATE SET
          tenant = excluded.tenant,
          projectName = excluded.projectName,
          description = excluded.description,
          startDate = excluded.startDate,
          endDate = excluded.endDate,
          status = excluded.status,
          customerName = excluded.customerName,
          customerEmail = excluded.customerEmail,
          customerMobile = excluded.customerMobile,
          lastChangeLogId = excluded.lastChangeLogId,
          createdOn = excluded.createdOn,
          isBlocked = excluded.isBlocked
      `);

      for (const entity of entitiesToSync) {
        stmt.run(entity);
      }

      //Update SQLite's lastChangeLogId
      const maxChangeLogId = Math.max(...entitiesToSync.map((e) => e.lastChangeLogId ?? 0));
      sqliteDb
        .prepare(`UPDATE project SET lastChangeLogId = ? WHERE id = ?`)
        .run(maxChangeLogId, projectId);

      console.log(`Synced ${entitiesToSync.length} entities from Postgres → SQLite (up to lastChangeLogId=${maxChangeLogId}).`
      );
    } else {
      console.log(
        `Project ${projectId} already in sync (lastChangeLogId=${pgLastId}).`
      );
    }
  }


}

/*
Project Sync process:
1) Postgresql query Project table.
2) Get lastChangeLogId from Postgresql. (integer)
3) Query ChangeLog Table with WHERE id > $1 AND project = $2
4) Save changelog query result in an array
5) Load sqlite database in sqlitedb variable

6) Loop over all entries in array
  Check entitiy name and create and update using DAO

7) Save sqlite
*/