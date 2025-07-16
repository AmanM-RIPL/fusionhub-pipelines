import { Insertable, Selectable, Transaction } from "kysely";
import { InsertableEntity, UpdateableEntity } from "../types/entity";

/*

Basic CRUD operations in this repository interface.

*/
export interface IBaseRepository<T> {
  findById(id: number): Promise<Selectable<T> | undefined>;
  findAll(limit: number, offset: number): Promise<Selectable<T>[]>;
  create(entity: InsertableEntity<T>): Promise<Selectable<T>>;
  update(id: number, updatedObject: UpdateableEntity<T>): Promise<Selectable<T> | undefined>;
  delete(id: number): Promise<void>;
}

/*

This repository interface is used when the delete operation is not allowed.

*/
export type IUnDeletableRepository<T> = Omit<IBaseRepository<T>, "delete">;