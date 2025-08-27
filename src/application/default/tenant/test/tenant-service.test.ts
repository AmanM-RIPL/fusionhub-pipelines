import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { TenantService } from "../tenant.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { ITenant } from "../tenant.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { UpdateableEntity } from "../../../common/types/entity";

describe('Default -> Tenant -> TenantService', () => {
  let tenantService: TenantService;
  let mockTenantRepository: DeepMockProxy<IUnDeletableRepository<ITenant>>;

  beforeEach(() => {
    mockTenantRepository = mockDeep<IUnDeletableRepository<ITenant>>();
    tenantService = new TenantService(mockTenantRepository);
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
    } as (Insertable<ITenant>);

    const insertableTenant = { name: newTenant.name, defaultEmail: newTenant.defaultEmail, defaultMobile: newTenant.defaultMobile } as Insertable<ITenant>;
    const createdTenant = { id: 2, name: newTenant.name, defaultEmail: newTenant.defaultEmail, defaultMobile: newTenant.defaultMobile } as Selectable<ITenant>;
    mockTenantRepository.create.mockResolvedValue(createdTenant);

    const result = await tenantService.create(newTenant);

    expect(mockTenantRepository.create).toHaveBeenCalledWith(insertableTenant);
    expect(result).toEqual(createdTenant);
  });

  test('create should call create repository method also', async () => {
    const newTenant: Insertable<ITenant> = {
      name: 'New Tenant',
      defaultEmail: 'sample@gmail.com',
      defaultMobile: '12345678'
    };

    const createdTenant: Selectable<ITenant> = {
      id: 2,
      createdOn: new Date(),
      name: newTenant.name,
      defaultEmail: newTenant.defaultEmail,
      defaultEmail1: null,
      defaultEmail2: null,
      defaultMobile: newTenant.defaultMobile,
      defaultMobile1: null,
      defaultMobile2: null,
      isBlocked: false,
    };
    mockTenantRepository.create.mockResolvedValue(createdTenant);

    const result = await tenantService.create(newTenant);

    expect(mockTenantRepository.create).toHaveBeenCalledWith(newTenant);
    expect(result).toEqual(createdTenant);
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