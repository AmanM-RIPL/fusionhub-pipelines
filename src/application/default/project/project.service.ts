import { Insertable, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { IProject } from "./project.model";
import { UpdateableEntity } from "../../common/types/entity";
import { IUserRepository } from "../user/user.repository";
import { IUser } from "../user/user.model";

export class ProjectService {
  constructor(protected readonly projectRepository: IUnDeletableRepository<IProject>, protected readonly userRespository: IUserRepository) {}

  async findById(id: number): Promise<Selectable<IProject> | undefined> {
    return await this.projectRepository.findById(id);
  }  

  async findAll(limit: number, offset: number): Promise<Selectable<IProject>[]> {
    return await this.projectRepository.findAll(limit, offset);
  }

  async create(projectDetails: Insertable<IProject>): Promise<Selectable<IProject>> {
    return await this.projectRepository.create(projectDetails);
  }

  async update(id: number, updatedProject: UpdateableEntity<IProject>): Promise<Selectable<IProject> | undefined> {
    return await this.projectRepository.update(id, updatedProject);
  }
}