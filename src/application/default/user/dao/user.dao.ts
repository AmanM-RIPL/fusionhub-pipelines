import { Insertable, Kysely, Selectable, Transaction } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IUser } from "../user.model";
import { IUserRepository } from "../user.repository";

/*

As user is a reserved word in postgreSQL, we use fh_user in the database.
However, at the application level, we refer to it as user.

*/

export class UserDao implements IUserRepository {

  constructor(protected readonly db: Kysely<IDatabase>, protected readonly tenant: number | null) {}

  async findById(id: number): Promise<Selectable<IUser> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing a user.");

    return await this.db.selectFrom("public.fh_user").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
  }

  async validateIds(ids: number[]): Promise<boolean> {
    if (this.tenant === null) throw new Error("Tenant must be set before validating user ids.");
    const result = await this.db
      .selectFrom("public.fh_user")
      .select("id")
      .where("tenant", "=", this.tenant)
      .where("id", "in", ids)
      .execute();
    return result.length === ids.length;
  }

  async findByUsernameAndPassword(username: string, password: string): Promise<Selectable<IUser> | undefined> {
    return await this.db
      .selectFrom("public.fh_user")
      .selectAll()
      .where("username", "=", username)
      .where("password", "=", password)
      .executeTakeFirst();
  }

  async findByUsername(username: string): Promise<Selectable<IUser> | undefined> {
    return await this.db
      .selectFrom("public.fh_user")
      .selectAll()
      .where("username", "=", username)
      .executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IUser>[]> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing a user.");

    return await this.db.selectFrom("public.fh_user").where("tenant", "=", this.tenant).selectAll().limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<IUser>): Promise<Selectable<IUser>> {
    if (this.tenant === null) throw new Error("Tenant must be set before creating a user.");

    const entityToInsert: Insertable<IUser> = {
      ...entity,
      tenant: this.tenant // Ensure tenant is set
    };
    return await this.db.insertInto("public.fh_user").values(entityToInsert).returningAll().executeTakeFirstOrThrow();
  }

  async update(id: number, updatedObject: UpdateableEntity<IUser>): Promise<Selectable<IUser> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before updating a user.");

    return await this.db
      .updateTable("public.fh_user")
      .set(updatedObject)
      .where("id", "=", id)
      .where("tenant", "=", this.tenant)
      .returningAll()
      .executeTakeFirst();
  }
}