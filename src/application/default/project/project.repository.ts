import { Selectable } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { IProject } from "./project.model"; 

export interface IProjectRepository extends IUnDeletableRepository<IProject> {
  
}