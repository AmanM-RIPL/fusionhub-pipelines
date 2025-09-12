import { Insertable, Kysely, Selectable, sql } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { ChangeHistory, InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IChangeLog } from "../change-log.model";
import { IChangeLogRepository } from "../change-log.repository";

/*

change_log = IChangeLog;

*/

export class ChangeLogDao implements IChangeLogRepository {

  constructor(
    protected readonly db: Kysely<IDatabase>, 
    protected readonly tenant: number | null,
    protected readonly user: number | null
  ) {}

  async findById(id: number): Promise<Selectable<IChangeLog> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    return await this.db.selectFrom("public.change_log").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IChangeLog>[]> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    return await this.db.selectFrom("public.change_log").where("tenant", "=", this.tenant).selectAll().limit(limit).offset(offset).execute();
  }

  async create(entity: InsertableEntity<IChangeLog>): Promise<Selectable<IChangeLog>> {
    if (this.tenant === null) throw new Error("Tenant must be set before creating an entity schema.");

    const entityToInsert: Insertable<IChangeLog> = {
      ...entity,
      tenant: this.tenant
    };
    return await this.db.insertInto("public.change_log").values(entityToInsert).returningAll().executeTakeFirstOrThrow();
  }

  update(id: number, updatedObject: UpdateableEntity<IChangeLog>): Promise<{ id: number; tenant: number; createdOn: Date; project: number; entity: string; createdByUser: number; entitySchema: any; associatedApprovedEntity: number | null; changeHistory: ChangeHistory; } | undefined> {
    throw new Error("Method not implemented.");
  }
  delete(id: number): Promise<void> {
    throw new Error("Method not implemented.");
  }
  
}