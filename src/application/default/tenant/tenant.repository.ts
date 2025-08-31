import { Selectable } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { ITenant } from "./tenant.model"; 

export interface ITenantRepository extends IUnDeletableRepository<ITenant> {
  
}