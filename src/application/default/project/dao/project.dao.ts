import { Insertable, Kysely, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IProject } from "../project.model";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IProjectRepository } from "../project.repository";

export class ProjectDao implements IProjectRepository {
   constructor(protected readonly db: Kysely<IDatabase>, protected readonly tenant: number | null) {} 

  async findById(id: number): Promise<Selectable<IProject> | undefined> {
    return await this.db.selectFrom("public.project").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
 }
  
  async findAll(limit: number, offset: number): Promise<Selectable<IProject>[]> {
    return await this.db.selectFrom("public.project").selectAll().where("tenant", "=", this.tenant).limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<IProject>): Promise<Selectable<IProject>> {
      if (this.tenant === null) throw new Error("Tenant must be set before creating a project.");
  
      const entityToInsert: Insertable<IProject> = {
        ...entity,
        tenant: this.tenant // Ensure tenant is set
      };
      return await this.db.insertInto("public.project").values(entityToInsert).returningAll().executeTakeFirstOrThrow();
    }

  async update(id: number, updatedObject: UpdateableEntity<IProject>): Promise<Selectable<IProject> | undefined> {
    return await this.db
      .updateTable("public.project")
      .set(updatedObject)
      .where("id", "=", id)
      .where("tenant", "=", this.tenant)
      .returningAll()
      .executeTakeFirst();
  }

 async findAllWithChangeLogGreaterThan( projectId: number, lastChangeLogId: number, limit: number, offset: number): Promise<Selectable<IProject>[]> {
  return await this.db
    .selectFrom("public.project")
    .selectAll()
    .where("id", "=", projectId)
    .where("tenant", "=", this.tenant)
    .where("lastChangeLogId", ">", lastChangeLogId)
    .limit(limit)
    .offset(offset)
    .execute();
 }

}