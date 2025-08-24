import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { TenantService } from "../tenant.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { ITenant } from "../tenant.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { UpdateableEntity } from "../../../common/types/entity";
import { IUserRepository } from "../../user/user.repository";
import { IUser } from "../../user/user.model";

describe('Default -> Tenant -> TenantService', () => {
  let tenantService: TenantService;
  let mockTenantRepository: DeepMockProxy<IUnDeletableRepository<ITenant>>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  beforeEach(() => {
    mockTenantRepository = mockDeep<IUnDeletableRepository<ITenant>>();
    mockUserRepository = mockDeep<IUserRepository>();
    tenantService = new TenantService(mockTenantRepository, mockUserRepository);
  });

  test('findById should call repository method with correct parameters', async () => {
    const mockTenant = { id: 1, name: 'Test Tenant' } as Selectable<ITenant>;
    mockTenantRepository.findById.mockResolvedValue(mockTenant);

    const result = await tenantService.findById(1);

    expect(mockTenantRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockTenant);
  });

  test('findAll should call repository method with correct parameters', async () => {
    const mockTenants = [{ id: 1, name: 'Test Tenant' }] as Selectable<ITenant>[];
    mockTenantRepository.findAll.mockResolvedValue(mockTenants);

    const result = await tenantService.findAll(10, 0);

    expect(mockTenantRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockTenants);
  });

  test('create should call repository method with correct parameters', async () => {
    const newTenant = { 
      name: 'New Tenant', 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '12345678', 
      adminUsername: 'sample', 
      adminPassword: 'sample' 
    } as (Insertable<ITenant> & { adminUsername: string, adminPassword: string });
    
    const insertableTenant = { name: newTenant.name, defaultEmail: newTenant.defaultEmail, defaultMobile: newTenant.defaultMobile } as Insertable<ITenant>;
    const createdTenant = { id: 2, name: newTenant.name, defaultEmail: newTenant.defaultEmail, defaultMobile: newTenant.defaultMobile } as Selectable<ITenant>;
    mockTenantRepository.create.mockResolvedValue(createdTenant);

    const result = await tenantService.create(newTenant);

    expect(mockTenantRepository.create).toHaveBeenCalledWith(insertableTenant);
    expect(result).toEqual(createdTenant);
  });

  test('create should call user-create repository method also', async () => {
    const newTenant = { 
      name: 'New Tenant', 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '12345678', 
      adminUsername: 'sample', 
      adminPassword: 'sample' 
    } as (Insertable<ITenant> & { adminUsername: string, adminPassword: string });
    
    const createdTenant = { id: 2, name: newTenant.name, defaultEmail: newTenant.defaultEmail, defaultMobile: newTenant.defaultMobile } as Selectable<ITenant>;
    
    const insertableUser: Insertable<IUser> = {
      username: newTenant.adminUsername,
      password: newTenant.adminPassword,
      tenant: createdTenant.id,
      email: createdTenant.defaultEmail,
      firstName: 'Tenant Admin',
      lastName: '',
      mobile: createdTenant.defaultMobile
    };
    const createdUser: Selectable<IUser> = { id: 3, createdOn: new Date(), tenant: createdTenant.id, ...insertableUser };

    mockUserRepository.create.mockResolvedValue(createdUser);
    mockTenantRepository.create.mockResolvedValue(createdTenant);

    const result = await tenantService.create(newTenant);

    expect(mockUserRepository.create).toHaveBeenCalledWith(insertableUser);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData = { name: 'Updated Tenant' } as UpdateableEntity<ITenant>;
    const updatedTenant = { id: 1, ...updatedData } as Selectable<ITenant>;
    mockTenantRepository.update.mockResolvedValue(updatedTenant);

    const result = await tenantService.update(1, updatedData);

    expect(mockTenantRepository.update).toHaveBeenCalledWith(1, updatedData);
    expect(result).toEqual(updatedTenant);
  });
});