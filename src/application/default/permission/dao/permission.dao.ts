import { Insertable, Kysely, Selectable, sql } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IPermission } from "../permission.model";
import { IBaseRepository } from "../../../common/repositories/base.repository";

/*

permission = IPermission;

*/

export class PermissionDao implements IBaseRepository<IPermission> {

  constructor(protected readonly db: Kysely<IDatabase>, protected readonly tenant: number | null) {}

  async findById(id: number): Promise<Selectable<IPermission> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an permission.");

    return await this.db.selectFrom("public.permission").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IPermission>[]> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an permission.");

    return await this.db.selectFrom("public.permission").where("tenant", "=", this.tenant).selectAll().limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<IPermission>): Promise<Selectable<IPermission>> {
    if (this.tenant === null) throw new Error("Tenant must be set before creating an permission.");

    const entityToInsert: Insertable<IPermission> = {
      ...entity,
      tenant: this.tenant
    };
    return await this.db.insertInto("public.permission").values(entityToInsert).returningAll().executeTakeFirstOrThrow();
  }

  async update(id: number, updatedObject: UpdateableEntity<IPermission>): Promise<Selectable<IPermission> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before updating an permission.");

    return await this.db
      .updateTable("public.permission")
      .set(updatedObject)
      .where("id", "=", id)
      .where("tenant", "=", this.tenant)
      .returningAll()
      .executeTakeFirst();
  }

  async delete(id: number): Promise<void> {
    if (this.tenant === null) throw new Error("Tenant must be set before deleting an permission.");

    await this.db.deleteFrom("public.permission").where("id", "=", id).where("tenant", "=", this.tenant).execute();
  }
}