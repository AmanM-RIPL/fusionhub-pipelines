import { Insertable, Kysely, Selectable, sql } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { ChangeHistory, InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IChangeLog } from "../change-log.model";
import { IChangeLogRepository } from "../change-log.repository";
import { IProject } from "../../project/project.model";

/*

change_log = IChangeLog;

*/

export class ChangeLogDao implements IChangeLogRepository {

  constructor(
    protected readonly db: Kysely<IDatabase>, 
    protected readonly tenant: number | null,
    protected readonly user: number | null
  ) {}
  insert(projectId: number): Promise<Selectable<IChangeLog>[]> {
    throw new Error("Method not implemented.");
  }

  async findById(id: number): Promise<Selectable<IChangeLog> | undefined> {
    if (this.tenant === null) throw new Error("Tenant must be set before accessing an entity schema.");

    return await this.db.selectFrom("public.change_log").selectAll().where("id", "=", id).where("tenant", "=", this.tenant).executeTakeFirst();
  }

 async findAllWithChangeLogGreaterThan( projectId: number, lastChangeLogId: number, limit: number): Promise<Selectable<IChangeLog>[]> {
    return await this.db
    .selectFrom("public.change_log")
    .selectAll()
    .where("project", "=", projectId)
    .where("tenant", "=", this.tenant)
    .where("id", ">", lastChangeLogId)
    .orderBy("id", "asc")
    .limit(limit)
    .execute();
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

  /*
    Not needed as of now. Will implement later if needed.
  */
  update(id: number, updatedObject: UpdateableEntity<IChangeLog>): Promise<{ id: number; tenant: number; createdOn: Date; project: number; entity: string; createdByUser: number; entitySchema: any; associatedApprovedEntity: number | null; changeHistory: ChangeHistory; } | undefined> {
    throw new Error("Method not implemented.");
  }

  /*
    Not needed as of now. Will implement later if needed.
  */
  delete(id: number): Promise<void> {
    throw new Error("Method not implemented.");
  }
  
}