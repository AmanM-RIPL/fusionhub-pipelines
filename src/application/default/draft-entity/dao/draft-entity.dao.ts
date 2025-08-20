import { Insertable, Kysely, Selectable, sql } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IDraftEntity } from "../draft-entity.model";
import { IDraftEntityRepository } from "../draft-entity.repository";

/*

draft_entity = IDraftEntity;

*/

export class DraftEntityDao implements IDraftEntityRepository {

  constructor(
    protected readonly db: Kysely<IDatabase>, 
    protected readonly tenant: number | null
  ) {}

  async findById(id: number): Promise<Selectable<IDraftEntity> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    return await this.db.selectFrom("public.draft_entity").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IDraftEntity>[]> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    return await this.db.selectFrom("public.draft_entity").where("tenant", "=", this.tenant).selectAll().limit(limit).offset(offset).execute();
  }

  async findBy(
    filters: { createdByUser?: number; entitySchema?: string; associatedApprovedEntity?: number; nextApprovingUser?: number },
  ): Promise<Selectable<IDraftEntity>[]> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    let query = this.db.selectFrom("public.draft_entity").selectAll();

    if (filters.createdByUser) query = query.where("createdByUser", "=", filters.createdByUser);
    if (filters.entitySchema) query = query.where("entitySchema", "=", filters.entitySchema);
    if (filters.associatedApprovedEntity) query = query.where("associatedApprovedEntity", "=", filters.associatedApprovedEntity);
    if (filters.nextApprovingUser) query = query.where("nextApprovingUser", "=", filters.nextApprovingUser);

    query = query.where("tenant", "=", this.tenant);

    return await query.execute();
  }

  async create(entity: InsertableEntity<IDraftEntity>): Promise<Selectable<IDraftEntity>> {
    if (this.tenant === null) throw new Error("Tenant must be set before creating an entity schema.");

    const entityToInsert: Insertable<IDraftEntity> = {
      ...entity,
      tenant: this.tenant
    };
    return await this.db.insertInto("public.draft_entity").values(entityToInsert).returningAll().executeTakeFirstOrThrow();
  }

  async update(id: number, updatedObject: Omit<UpdateableEntity<IDraftEntity>, "entitySchema" | "createdByUser" | "associatedApprovedEntity">): Promise<Selectable<IDraftEntity> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before updating an entity schema.");

    return await this.db
      .updateTable("public.draft_entity")
      .set(updatedObject)
      .where("id", "=", id)
      .where("tenant", "=", this.tenant)
      .returningAll()
      .executeTakeFirst();
  }

  async delete(id: number): Promise<void> {
    if (this.tenant === null) throw new Error("Tenant must be set before deleting an entity schema.");

    await this.db.deleteFrom("public.draft_entity").where("id", "=", id).where("tenant", "=", this.tenant).execute();
  }
}