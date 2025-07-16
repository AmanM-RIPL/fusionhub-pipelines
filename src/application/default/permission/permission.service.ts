import { Selectable } from "kysely";
import { IPermission } from "./permission.model";
import { AccessPermission, FilterPermission, InsertableEntity, UpdateableEntity } from "../../common/types/entity";
import { IBaseRepository } from "../../common/repositories/base.repository";
import { IUserRepository } from "../user/user.repository";

export class PermissionService {
  constructor(
    protected readonly permissionRepository: IBaseRepository<IPermission>,
    protected readonly userRepository: IUserRepository
  ) {}

  async findById(id: number): Promise<Selectable<IPermission> | undefined> {
    return await this.permissionRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IPermission>[]> {
    return await this.permissionRepository.findAll(limit, offset);
  }

  async create(entity: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission }): Promise<Selectable<IPermission>> {
    // Approval number[] cannot have same user multiple times
    if (entity.approval.length !== new Set(entity.approval).size) {
      throw new Error("Approval array cannot contain duplicate users.");
    }

    // Validate if all users exist in the user table
    if (!(await this.userRepository.validateIds(entity.approval))) {
      throw new Error("One or more users in the approval array do not exist.");
    }

    /*
    
    Update once tables used in filter are created.
    Need to validate if the ids in filter exist in the respective tables.
    
    */

    // JSONify filter and access properties
    const entityToInsert = { ...entity, filter: '', access: '' } as InsertableEntity<IPermission>;
    entityToInsert.filter = JSON.stringify(entity.filter);
    entityToInsert.access = JSON.stringify(entity.access);

    return await this.permissionRepository.create(entityToInsert);
  }

  async update(id: number, updatedObject: Omit<InsertableEntity<IPermission>, 'filter' | 'access' | 'user' | 'entity'> & { filter: FilterPermission, access: AccessPermission }): Promise<Selectable<IPermission> | undefined> {
    // Approval number[] cannot have same user multiple times
    if (updatedObject.approval && updatedObject.approval.length !== new Set(updatedObject.approval).size) {
      throw new Error("Approval array cannot contain duplicate users.");
    }

    // Validate if all users exist in the user table
    if (updatedObject.approval && !(await this.userRepository.validateIds(updatedObject.approval))) {
      throw new Error("One or more users in the approval array do not exist.");
    }

    /*
    
    Update once tables used in filter are created.
    Need to validate if the ids in filter exist in the respective tables.
    
    */

    // JSONify filter and access properties
    const updatedObjectToUpdate = { ...updatedObject, filter: '', access: '' } as UpdateableEntity<IPermission>;
    updatedObjectToUpdate.filter = JSON.stringify(updatedObject.filter);
    updatedObjectToUpdate.access = JSON.stringify(updatedObject.access);

    return await this.permissionRepository.update(id, updatedObjectToUpdate);
  }

  async delete(id: number): Promise<void> {
    await this.permissionRepository.delete(id);
  }
}