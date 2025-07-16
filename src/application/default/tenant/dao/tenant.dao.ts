import { Insertable, Kysely, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { ITenant } from "../tenant.model";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";

export class TenantDao implements IUnDeletableRepository<ITenant> {
  constructor(protected readonly db: Kysely<IDatabase>) {}

  async findById(id: number): Promise<Selectable<ITenant> | undefined> {
    return await this.db.selectFrom("public.tenant").selectAll().where("id", "=", id).executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<ITenant>[]> {
    return await this.db.selectFrom("public.tenant").selectAll().limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<ITenant>): Promise<Selectable<ITenant>> {
    return await this.db.insertInto("public.tenant").values(entity).returningAll().executeTakeFirstOrThrow();
  }

  async update(id: number, updatedObject: UpdateableEntity<ITenant>): Promise<Selectable<ITenant> | undefined> {
    return await this.db
      .updateTable("public.tenant")
      .set(updatedObject)
      .where("id", "=", id)
      .returningAll()
      .executeTakeFirst();
  }
}