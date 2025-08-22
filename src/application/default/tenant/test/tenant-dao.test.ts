import { InsertQueryBuilder, InsertResult, Kysely, Selectable, SelectQueryBuilder, Transaction, UpdateQueryBuilder } from "kysely";
import { TenantDao } from "../dao/tenant.dao";
import { mockDeep, DeepMockProxy } from 'jest-mock-extended';
import { IDatabase } from "../../../../infrastructure/db/kysely/types";

// Helper types for the chained query builder objects
type TenantsSelectQueryBuilder = SelectQueryBuilder<IDatabase, 'public.tenant', {}>;
type TenantsInsertQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, InsertResult>;
type TenantsInsertReturningAllQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, Selectable<IDatabase['public.tenant']>>;
type TenantsUpdateReturningAllQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.tenant', 'public.tenant', Selectable<IDatabase['public.tenant']>>;
type TenantsUpdateQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.tenant', 'public.tenant', {}>;

describe('Default -> Tenant -> TenantDao', () => {
  let mockKysely: DeepMockProxy<Kysely<IDatabase>>;
  let tenantDao: TenantDao;

  beforeEach(() => {
    mockKysely = mockDeep<Kysely<IDatabase>>();
    tenantDao = new TenantDao(mockKysely);
  });

  test('findById should call correct methods', async () => {
    // test data
    const mockTenant = { id: 1, name: 'John Doe' };;

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<TenantsSelectQueryBuilder>();
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
    const mockTenant = { id: 1, name: 'John Doe' };;

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<TenantsSelectQueryBuilder>();
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
    const mockTenants = [{ id: 1, name: 'John Doe' }, { id: 2, name: 'Jane Doe' }];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<TenantsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockTenants);


    // Call the method under test
    const tenantResult = await tenantDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.tenant");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.limit).toHaveBeenCalledWith(10);
    expect(mockSelect.offset).toHaveBeenCalledWith(0);
    expect(mockSelect.execute).toHaveBeenCalled();
  });

  test('findAll should return correct value', async () => {
    // test data
    const mockTenants = [{ id: 1, name: 'John Doe' }, { id: 2, name: 'Jane Doe' }];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<TenantsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockTenants);


    // Call the method under test
    const tenantResult = await tenantDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenants);
  });

  test('create should call correct methods', async () => {
    // test data
    const mockTenants = { 
      id: 1, 
      name: 'John Doe', 
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
    const mockInsert = mockDeep<TenantsInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<TenantsInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockTenants);


    // Call the method under test
    const tenantResult = await tenantDao.create(mockTenants);

    // Add your assertions or method calls here
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.tenant");
    expect(mockInsert.values).toHaveBeenCalledWith(mockTenants);
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockTenants = { 
      id: 1, 
       name: 'John Doe', 
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
    const mockInsert = mockDeep<TenantsInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<TenantsInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockTenants);


    // Call the method under test
    const tenantResult = await tenantDao.create(mockTenants);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenants);
  });

  test('update should call correct methods', async () => {
    // test data
    const mockTenants = { 
      id: 1, 
      name: 'John Doe', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',
      defaultEmail1: null, 
      defaultEmail2: null, 
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false 
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<TenantsUpdateQueryBuilder>();
    //@ts-ignore
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<TenantsUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockTenants);    

    // Call the method under test
    const tenantResult = await tenantDao.update(1, mockTenants);

    // Add your assertions or method calls here
    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.tenant");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockTenants);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
    // test data
    const mockTenants = { 
      id: 1, 
      name: 'John Doe', 
      createdOn: new Date(), 
      defaultEmail: 'sample@gmail.com',
      defaultEmail1: null, 
      defaultEmail2: null, 
      defaultMobile: '1234567890',
      defaultMobile1: null, 
      defaultMobile2: null, 
      isBlocked: false 
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<TenantsUpdateQueryBuilder>();
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<TenantsUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockTenants);    

    // Call the method under test
    const tenantResult = await tenantDao.update(1, mockTenants);

    // Add your assertions or method calls here
    expect(tenantResult).toEqual(mockTenants);
  });
});