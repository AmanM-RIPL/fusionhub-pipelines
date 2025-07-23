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

  async create(projectDetails: Insertable<IProject> & { adminUsername: string, adminPassword: string }): Promise<Selectable<IProject>> {
    // need to remove additional properties from projectDetails
    const { adminUsername, adminPassword, ...project } = projectDetails;
    const newProject = await this.projectRepository.create(project);

    // create the default user for the tenant
    const insertableUser: Insertable<IUser> = {
      username: projectDetails.adminUsername,
      password: projectDetails.adminPassword,
      tenant: newProject.id,
      email: project.contactEmail ?? '',
      firstName: 'Tenant Admin',
      lastName: '',
      mobile :  ''
    };
    const newUser = await this.userRespository.create(insertableUser);
    return newProject;
  }

  async update(id: number, updatedProject: UpdateableEntity<IProject>): Promise<Selectable<IProject> | undefined> {
    return await this.projectRepository.update(id, updatedProject);
  }
}