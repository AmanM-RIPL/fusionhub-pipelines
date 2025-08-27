import { Selectable } from "kysely";
import { IDraftEntityRepository } from "./draft-entity.repository";
import { IDraftEntity } from "./draft-entity.model";
import { InsertableEntity, UpdateableEntity } from "../../common/types/entity";

export class DraftEntityService {
  constructor(
    protected readonly draftEntityRepository: IDraftEntityRepository,
    protected readonly user: number
  ) {}

  async findById(id: number): Promise<Selectable<IDraftEntity> | undefined> {
    return await this.draftEntityRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IDraftEntity>[]> {
     const result = await this.draftEntityRepository.findAll(limit, offset);
  
    return await this.draftEntityRepository.findAll(limit, offset);
  }

  async findBy(filters: { createdByUser?: number; entitySchema?: string; associatedApprovedEntity?: number; nextApprovingUser?: number }): Promise<Selectable<IDraftEntity>[]> {
    return await this.draftEntityRepository.findBy(filters);
  }

  async create(
    entity: Omit<InsertableEntity<IDraftEntity>, "createdByUser" | "nextApprovingUser" | "changeHistory">
  ): Promise<Selectable<IDraftEntity>> {
    const entityToCreate: InsertableEntity<IDraftEntity> = {
      ...entity,
      createdByUser: this.user,
      nextApprovingUser: null,
      changeHistory: JSON.stringify({
        user: this.user,
        description: "",
        timestamp: new Date(),
        approvalHistory: []
      })
    };
    return await this.draftEntityRepository.create(entityToCreate);
  }

  async update(id: number, updatedObject: { [key: string]: unknown }): Promise<Selectable<IDraftEntity> | undefined> {
    return await this.draftEntityRepository.update(id, updatedObject);
  }

  // approvalHeirarcy = [1,2,3];
  async approve(id: number, approvalHierarcy: number[]): Promise<Selectable<IDraftEntity> | undefined> {
    const draftEntity = await this.draftEntityRepository.findById(id);
    if (!draftEntity) {
      throw new Error(`Draft entity with id ${id} not found.`);
    }

    if (draftEntity.nextApprovingUser !== this.user) {
      throw new Error(`User ${this.user} is not the next approving user for draft entity with id ${id}.`);
    }

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
