import { Selectable } from "kysely";
import { IChangeLogRepository } from "./change-log.repository";
import { IChangeLog } from "./change-log.model";
import { InsertableEntity, UpdateableEntity } from "../../common/types/entity";

export class ChangeLogService {
  constructor(
    protected readonly changeLogRepository: IChangeLogRepository,
    protected readonly user: number
  ) { }

  async findById(id: number): Promise<Selectable<IChangeLog> | undefined> {
    return await this.changeLogRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IChangeLog>[]> {
    return await this.changeLogRepository.findAll(limit, offset);
  }

  /*
    Added for just now. Will remove later. Change log need to automatically created in draftEntityService.approved
  */
  async create(entity: Omit<InsertableEntity<IChangeLog>, "createdByUser" | "changeHistory">): Promise<Selectable<IChangeLog>> {
    const entityToCreate: InsertableEntity<IChangeLog> = {
      ...entity,
      createdByUser: this.user,
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

    return await this.changeLogRepository.create(entityToCreate);
  }

}
