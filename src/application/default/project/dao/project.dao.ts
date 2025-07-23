import { Insertable, Kysely, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IProject } from "../project.model";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";

export class ProjectDao implements IUnDeletableRepository<IProject> {
  constructor(protected readonly db: Kysely<IDatabase>) {}

  async findById(id: number): Promise<Selectable<IProject> | undefined> {
    return await this.db.selectFrom("public.project").selectAll().where("id", "=", id).executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IProject>[]> {
    return await this.db.selectFrom("public.project").selectAll().limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<IProject>): Promise<Selectable<IProject>> {
    return await this.db.insertInto("public.project").values(entity).returningAll().executeTakeFirstOrThrow();
  }

  async update(id: number, updatedObject: UpdateableEntity<IProject>): Promise<Selectable<IProject> | undefined> {
    return await this.db
      .updateTable("public.project")
      .set(updatedObject)
      .where("id", "=", id)
      .returningAll()
      .executeTakeFirst();
  }
}