import { Insertable, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { ITenant } from "./tenant.model";
import { UpdateableEntity } from "../../common/types/entity";

export class TenantService {
  constructor(protected readonly tenantRepository: IUnDeletableRepository<ITenant>) {}

  async findById(id: number): Promise<Selectable<ITenant> | undefined> {
    return await this.tenantRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<ITenant>[]> {
    return await this.tenantRepository.findAll(limit, offset);
  }

  async create(tenantDetails: Insertable<ITenant>): Promise<Selectable<ITenant>> {
    
    const { ...tenant } = tenantDetails;
    const newTenant = await this.tenantRepository.create(tenant);
    return newTenant;
  }

  async update(id: number, updatedTenant: UpdateableEntity<ITenant>): Promise<Selectable<ITenant> | undefined> {
    return await this.tenantRepository.update(id, updatedTenant);
  }
}