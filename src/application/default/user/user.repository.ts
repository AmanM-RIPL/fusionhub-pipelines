import { Selectable } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { IUser } from "./user.model";
import { InsertableEntity } from "../../common/types/entity";

export interface IUserRepository extends IUnDeletableRepository<IUser> {
  findByUsernameAndPassword(username: string, password: string): Promise<Selectable<IUser> | undefined>;
  findByUsername(username: string): Promise<Selectable<IUser> | undefined>;
  validateIds(ids: number[]): Promise<boolean>;
  adminCreate(entity: InsertableEntity<IUser>): Promise<Selectable<IUser>>;
}