import { Kysely, Selectable } from "kysely";
import { TenantDao } from "../dao/tenant.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyDeleteQueryBuilder, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder, MockKyselyUpdateQueryBuilder } from "../../../common/types/test";

describe('Default -> Tenant -> TenantDao', () => {
  let mockKysely: MockKysely;
  let tenantDao: TenantDao; 
  let tenant = 2;
  let user = 2;

  beforeEach(() => {
    mockKysely = {
      selectFrom: jest.fn(),
      deleteFrom: jest.fn(),
      insertInto: jest.fn(),
      updateTable: jest.fn(),
    };

    tenantDao = new TenantDao(mockKysely  as unknown as Kysely<IDatabase>);
  });

  test('findById should call correct methods', async () => {
    const mockTenant = { id: 1, entity: 'tenant' };

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockTenant);

    // Call the method under test
    const tenantResult = await tenantDao.findById(1);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.tenant");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockTenant = { id: 1, entity: 'tenant' };

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockTenant);

    // Call the method under test
    const tenantResult = await tenantDao.findById(1);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenant);
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockTenants = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

    // Mock the Kysely methods with a simplified approach
    const mockQueryBuilder: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };

    mockKysely.selectFrom.mockReturnValue(mockQueryBuilder);
    mockQueryBuilder.where.mockReturnThis();
    mockQueryBuilder.selectAll.mockReturnThis();
    mockQueryBuilder.limit.mockReturnThis();
    mockQueryBuilder.offset.mockReturnThis();
    mockQueryBuilder.execute.mockResolvedValueOnce(mockTenants);

    // Call the method under test
    const tenantResult = await tenantDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.tenant");
    expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
    expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
    expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
    expect(mockQueryBuilder.execute).toHaveBeenCalled();
  });

  test('create should call correct methods', async () => {
    // test data
    const mockTenants: Selectable<IDatabase['public.tenant']> = {
      id: 1,
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    const mockTenantInsertable: InsertableEntity<IDatabase['public.tenant']> = {
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockInsert: MockKyselyInsertQueryBuilder = {
      values: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
    };

    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    mockInsert.returningAll.mockReturnThis();
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockTenants);

    // Call the method under test
    const tenantResult = await tenantDao.create(mockTenantInsertable);

    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.tenant");
    expect(mockInsert.values).toHaveBeenCalledWith({
      ...mockTenantInsertable,
    });
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockInsert.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockTenants: Selectable<IDatabase['public.tenant']> = {
      id: 1, 
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false    };

    const mockTenantInsertable: InsertableEntity<IDatabase['public.tenant']> = { 
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockInsert: MockKyselyInsertQueryBuilder = {
      values: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
    };
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    mockInsert.returningAll.mockReturnThis();
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockTenants);

    // Call the method under test
    const tenantResult = await tenantDao.create(mockTenantInsertable);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenants);
  });

  test('update should call correct methods', async () => {
    // test data
     const mockTenants = {
      id: 1, 
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false    };

    const mockTenantUpdateable = { 
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate: MockKyselyUpdateQueryBuilder = {
      set: jest.fn(),
      where: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      executeTakeFirst: jest.fn(),
    };
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    mockUpdate.returningAll.mockReturnThis();
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockTenants);

    // Call the method under test
    const tenantResult = await tenantDao.update(1, mockTenantUpdateable);

    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.tenant");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockTenantUpdateable);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockUpdate.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
    // test data
    const mockTenants: Selectable<IDatabase['public.tenant']> = {
       id: 1, 
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    const mockTenantUpdateable:
      Omit<
        UpdateableEntity<IDatabase['public.tenant']>,
        "entitySchema" | "createdByUser" | "associatedApprovedEntity"
      > = {
      name: 'NMBPL', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',  
      defaultEmail1: 'sample1@gmail.com',  
      defaultEmail2: 'sample2@gmail.com',  
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate: MockKyselyUpdateQueryBuilder = {
      set: jest.fn(),
      where: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      executeTakeFirst: jest.fn(),
    };
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    mockUpdate.returningAll.mockReturnThis();
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockTenants);

    // Call the method under test
    const tenantResult = await tenantDao.update(1, mockTenantUpdateable);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenants);
  });

});