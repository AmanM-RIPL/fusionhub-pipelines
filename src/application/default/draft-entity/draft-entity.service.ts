import { Selectable } from "kysely";
import { IDraftEntityRepository } from "./draft-entity.repository";
import { IDraftEntity } from "./draft-entity.model";
import { InsertableEntity, UpdateableEntity } from "../../common/types/entity";
import { IChangeLog } from "../change-log/change-log.model";
import { IChangeLogRepository } from "../change-log/change-log.repository";

export class DraftEntityService {
  constructor(
    protected readonly draftEntityRepository: IDraftEntityRepository,
    protected readonly changeLogRepository: IChangeLogRepository,
    protected readonly user: number
  ) { }

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

  async create(entity: Omit<InsertableEntity<IDraftEntity>, "createdByUser" | "nextApprovingUser" | "changeHistory">): Promise<Selectable<IDraftEntity>> {
    const entityToCreate: InsertableEntity<IDraftEntity> = {
      ...entity,
      createdByUser: this.user,
      nextApprovingUser: null,
      changeHistory: JSON.stringify({
          user: this.user,
          changeType: "create",
          description: "Approved By User",
          timestamp: new Date(),
          approvalHistory: [
            {
              user: this.user,
              timestamp: new Date(),
              description: "Approved By User",
              status: "approved"
            }
          ]
        })
    };

    return await this.draftEntityRepository.create(entityToCreate);
  }

  async update(id: number, updatedObject: { [key: string]: unknown }): Promise<Selectable<IDraftEntity> | undefined> {
    return await this.draftEntityRepository.update(id, updatedObject);
  }

  async approve(id: number, approvalHierarchy: number[]): Promise<Selectable<IDraftEntity> | Selectable<IChangeLog> | undefined> {
    const draftEntity = await this.draftEntityRepository.findById(id);
    if (!draftEntity) {
      throw new Error(`Draft entity with id ${id} not found.`);
    }

    if (draftEntity.nextApprovingUser !== this.user) {
      throw new Error(
        `User ${this.user} is not the next approving user for draft entity with id ${id}.`
      );
    }

    // Last approver
    if (approvalHierarchy.length === 0 || approvalHierarchy[approvalHierarchy.length - 1] === this.user) {
      const changeLog = await this.changeLogRepository.create({
        project: draftEntity.project,
        entity: draftEntity.entity,
        entitySchema: draftEntity.entitySchema,
        createdByUser: this.user,
        changeHistory: JSON.stringify({
          ...draftEntity.changeHistory,
          approvalHistory: [
            ...draftEntity.changeHistory.approvalHistory,
            {
              user: this.user,
              timestamp: new Date(),
              description: "Approved By User",
              status: "approved"
            }
          ],
        }),
      });

      await this.draftEntityRepository.delete(id);
      return changeLog;
    }

    // if Next approver
    const currentIndex = approvalHierarchy.indexOf(this.user);
    if (currentIndex === -1 || !approvalHierarchy[currentIndex + 1]) {
      throw new Error(`Invalid approval hierarchy for user ${this.user}`);
    }
    const nextApprovingUser = approvalHierarchy[currentIndex + 1];

   return await this.draftEntityRepository.approvingUpdate(id, {
      nextApprovingUser,
      changeHistory: {
        ...draftEntity.changeHistory,
        approvalHistory: [
          ...draftEntity.changeHistory.approvalHistory,
          {
            user: this.user,
            timestamp: new Date(),
            description: "Approved By User",
            status: "approved"
          }
        ],
      },
    });
  }
 
  async delete(id: number): Promise<void> {
    await this.draftEntityRepository.delete(id);
  }
}
