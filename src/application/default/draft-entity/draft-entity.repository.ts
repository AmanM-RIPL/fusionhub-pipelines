import { Selectable } from "kysely";
import { IBaseRepository } from "../../common/repositories/base.repository";
import { IDraftEntity } from "./draft-entity.model";
import { ChangeHistory } from "../../common/types/entity";

export interface IDraftEntityRepository extends IBaseRepository<IDraftEntity> {
  findBy(
    filters: { 
      createdByUser?: number; 
      entitySchema?: string; 
      associatedApprovedEntity?: number; 
      nextApprovingUser?: number 
    }
  ): Promise<Selectable<IDraftEntity>[]>;

  approvingUpdate(id: number, updatedObject: { nextApprovingUser: number, changeHistory: ChangeHistory }): Promise<Selectable<IDraftEntity> | undefined>;
}