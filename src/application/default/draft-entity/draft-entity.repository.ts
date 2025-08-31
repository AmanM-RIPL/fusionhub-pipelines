import { Selectable } from "kysely";
import { IBaseRepository } from "../../common/repositories/base.repository";
import { IDraftEntity } from "./draft-entity.model";

export interface IDraftEntityRepository extends IBaseRepository<IDraftEntity> {
  findBy(
    filters: { 
      createdByUser?: number; 
      entitySchema?: string; 
      associatedApprovedEntity?: number; 
      nextApprovingUser?: number 
    }
  ): Promise<Selectable<IDraftEntity>[]>;
  approve(id: number, approvalHierarcy: number[]): Promise<Selectable<IDraftEntity>>;
}