import { Selectable } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { IPermission } from "./permission.model"; 

export interface IPermissionRepository extends IUnDeletableRepository<IPermission> {
  
}