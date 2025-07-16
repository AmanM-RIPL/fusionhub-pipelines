import { Selectable } from "kysely";
import { IDraftEntityRepository } from "./draft-entity.repository";
import { IDraftEntity } from "./draft-entity.model";
import { ColumnValue, InsertableEntity, UpdateableEntity } from "../../common/types/entity";

export class DraftEntityService {
  constructor(
    protected readonly draftEntityRepository: IDraftEntityRepository,
    protected readonly user: number
  ) {}

  async findById(id: number): Promise<Selectable<IDraftEntity> | undefined> {
    return await this.draftEntityRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IDraftEntity>[]> {
    return await this.draftEntityRepository.findAll(limit, offset);
  }

  async findBy(filters: { createdByUser?: number; entitySchema?: string; associatedApprovedEntity?: number; parentDraftEntity?: number; nextApprovingUser?: number }): Promise<Selectable<IDraftEntity>[]> {
    return await this.draftEntityRepository.findBy(filters);
  }

  async create(entity: Omit<InsertableEntity<IDraftEntity>, "createdByUser" | "nextApprovingUser" | "changeHistory" | "data"> & { data: ColumnValue }): Promise<Selectable<IDraftEntity>> {
    
    const entityToCreate: InsertableEntity<IDraftEntity> = {
      ...entity,
      createdByUser: this.user,
      nextApprovingUser: null,
      data: JSON.stringify(entity.data),
      changeHistory: JSON.stringify({
        user: this.user,
        changeType: entity.parentDraftEntity ? "update" : "create",
        description: "",
        timestamp: new Date(),
        approvalHistory: []
      })
    };

    return await this.draftEntityRepository.create(entityToCreate);
  }

  async update(id: number, updatedObject: { data: ColumnValue }): Promise<Selectable<IDraftEntity> | undefined> {
    const updatedData = {
      data: JSON.stringify(updatedObject.data),
    };
    return await this.draftEntityRepository.update(id, updatedData);
  }

  async delete(id: number): Promise<void> {
    // get all entities that have the parentDraftEntity set to this id
    const childEntities = await this.draftEntityRepository.findBy({ parentDraftEntity: id });

    // delete all child entities
    for (const child of childEntities) {
      await this.draftEntityRepository.delete(child.id);
    }

    // delete the draft entity itself
    return await this.draftEntityRepository.delete(id);
  }

  async approve(id: number, approvalHierarcy: number[]): Promise<Selectable<IDraftEntity> | undefined> {
    const draftEntity = await this.draftEntityRepository.findById(id);
   
    if (!draftEntity) {
      throw new Error(`Draft entity with id ${id} not found.`);
    }
    
    if (draftEntity.nextApprovingUser !== this.user) {
      throw new Error(`User ${this.user} is not the next approving user for draft entity with id ${id}.`);
    }

    // if current user is the last in the approval hierarchy, mark the draft entity as approved
    if (approvalHierarcy.length === 0 || approvalHierarcy[approvalHierarcy.length - 1] === this.user) {
      return {
        ...draftEntity,
        changeHistory: {
          ...draftEntity.changeHistory,
          approvalHistory: [
            ...draftEntity.changeHistory.approvalHistory,
            {
              user: this.user,
              timestamp: new Date(),
              description: "Approved by user",
              status: "approved"
            }
          ]
        }
      };
    } else {
      // find the next user in the approval hierarchy
      const currentIndex = approvalHierarcy.indexOf(this.user);
      await this.draftEntityRepository.update(id, {
        nextApprovingUser: approvalHierarcy[currentIndex + 1],
        changeHistory: JSON.stringify({
          ...draftEntity.changeHistory,
          approvalHistory: [
            ...draftEntity.changeHistory.approvalHistory,
            {
              user: this.user,
              timestamp: new Date(),
              description: "Approved by user",
              status: "approved"
            }
          ]
        })
      });
    }
  }
}