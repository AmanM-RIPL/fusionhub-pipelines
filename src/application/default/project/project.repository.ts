import { Selectable } from "kysely";
import { IBaseRepository, IUnDeletableRepository } from "../../common/repositories/base.repository";
import { IProject } from "./project.model"; 

export interface IProjectRepository extends IUnDeletableRepository<IProject> {

  findAllWithChangeLogGreaterThan(projectId: number, lastChangeLogId: number, limit:number, offset:number): Promise<Selectable<IProject>[]>;

}
