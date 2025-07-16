import { Insertable, Selectable, Transaction } from "kysely";
import { IUnDeletableRepository } from "../../common/repositories/base.repository";
import { ITenant } from "./tenant.model";
import { UpdateableEntity } from "../../common/types/entity";
import { IUserRepository } from "../user/user.repository";
import { IUser } from "../user/user.model";

export class TenantService {
  constructor(protected readonly tenantRepository: IUnDeletableRepository<ITenant>, protected readonly userRespository: IUserRepository) {}

  async findById(id: number): Promise<Selectable<ITenant> | undefined> {
    return await this.tenantRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<ITenant>[]> {
    return await this.tenantRepository.findAll(limit, offset);
  }

  async create(tenantDetails: Insertable<ITenant> & { adminUsername: string, adminPassword: string }): Promise<Selectable<ITenant>> {
    // need to remove additional properties from tenantDetails
    const { adminUsername, adminPassword, ...tenant } = tenantDetails;
    const newTenant = await this.tenantRepository.create(tenant);

    // create the default user for the tenant
    const insertableUser: Insertable<IUser> = {
      username: tenantDetails.adminUsername,
      password: tenantDetails.adminPassword,
      tenant: newTenant.id,
      email: tenant.defaultEmail,
      firstName: 'Tenant Admin',
      lastName: '',
      mobile: tenant.defaultMobile
    };
    
    const newUser = await this.userRespository.create(insertableUser);

    return newTenant;
  }

  async update(id: number, updatedTenant: UpdateableEntity<ITenant>): Promise<Selectable<ITenant> | undefined> {
    return await this.tenantRepository.update(id, updatedTenant);
  }
}