import { Insertable, Selectable, Transaction } from "kysely";
import { IProject } from "./project.model";
import { UpdateableEntity } from "../../common/types/entity";
import { IProjectRepository } from "../project/project.repository";
import { sqliteTable } from "../../../infrastructure/db/dbsqlite/sqliteSchema"
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
    const __dirname = 'C:/code/fhapi_service/src/infrastructure/sqlite/';
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
        sqliteDb.close();
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

}
