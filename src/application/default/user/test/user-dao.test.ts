import { Kysely, Selectable } from "kysely";
import { UserDao } from "../dao/user.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyDeleteQueryBuilder, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder, MockKyselyUpdateQueryBuilder } from "../../../common/types/test";

describe('Default -> User -> UserDao', () => {
    let mockKysely: MockKysely;
    let userDao: UserDao;
    let userDaoNoTenant: UserDao;
    let tenant = 2;
    let user = 2;

  beforeEach(() => {
    mockKysely = {
      selectFrom: jest.fn(),
      deleteFrom: jest.fn(),
      insertInto: jest.fn(),
      updateTable: jest.fn(),
    };
    
    userDao = new UserDao(mockKysely  as unknown as Kysely<IDatabase>, tenant);
    userDaoNoTenant = new UserDao(mockKysely  as unknown as Kysely<IDatabase>, null);  
  });

  test('findById should call correct methods', async () => {
    const mockUser = { id: 1, entity: 'tenant' };
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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockUser);

    // Call the method under test
    const userResult = await userDao.findById(1);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should call correct value', async () => {
    const mockUser = {  
      id: 1, 
      tenant: tenant,
      createdOn: new Date(),
      username: 'JohnDoe', 
      password: 'password123', 
      email: 'sample@gmail.com', 
      firstName: 'John', 
      lastName: 'Doe', 
      mobile: '1234567890' };

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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockUser);

    const userResult = await userDao.findById(1);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();

    expect(userResult).toEqual(mockUser);

  });

  test('findById should return error if tenant = null', async () => {
    async function userResult() {
      await userDaoNoTenant.findById(1);
    }

    await expect(userResult).rejects.toThrow("Tenant must be set before accessing a user.");
  });

   test('findAll should call correct methods', async () => {
    // test data
    const mockUsers = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

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
    mockQueryBuilder.execute.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.findAll(10, 0);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
    expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
    expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
    expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
    expect(mockQueryBuilder.execute).toHaveBeenCalled();
  });

  test('findAll should call correct value', async () => {
    // test data
    const mockUsers = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

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
    mockQueryBuilder.execute.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.findAll(10, 0);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
    expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
    expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
    expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
    expect(mockQueryBuilder.execute).toHaveBeenCalled();
  });

  test('findAll should return error if tenant = null', async () => {
    // Call the method under test
    async function userResult() {
      await userDaoNoTenant.findAll(10, 0);
    }

    // Add assertions.
    await expect(userResult).rejects.toThrow("Tenant must be set before accessing a user.");
  });

   test('create should call correct methods', async () => {
    // test data
    const mockUsers: Selectable<IDatabase['public.fh_user']> = {
        id: 1, 
        tenant: tenant,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
    };

    const mockUserInsertable: InsertableEntity<IDatabase['public.fh_user']> = {
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.create(mockUserInsertable);

    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.fh_user");
    expect(mockInsert.values).toHaveBeenCalledWith({
      ...mockUserInsertable,
      tenant: tenant
    });
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockInsert.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockUsers: Selectable<IDatabase['public.fh_user']> = {
        id: 1, 
        tenant: tenant,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
    };

    const mockUserInsertable: InsertableEntity<IDatabase['public.fh_user']> = {
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.create(mockUserInsertable);

    // Add assertions.
    expect(userResult).toEqual(mockUsers);
  });


   test('update should call correct methods', async () => {
    // test data
    const mockUsers: Selectable<IDatabase['public.fh_user']> = {
        id: 1, 
        tenant: tenant,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
    };

    const mockUserUpdateable: UpdateableEntity<IDatabase['public.fh_user']> = {
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
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
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.update(1, mockUserUpdateable);

    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.fh_user");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockUserUpdateable);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockUpdate.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should call correct Value', async () => {
    // test data
    const mockUsers: Selectable<IDatabase['public.fh_user']> = {
        id: 1, 
        tenant: tenant,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
    };

    const mockUserUpdateable: UpdateableEntity<IDatabase['public.fh_user']> = {
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890'
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
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockUsers);

    // Call the method under test
    const userResult = await userDao.update(1, mockUserUpdateable);

    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.fh_user");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockUserUpdateable);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockUpdate.executeTakeFirst).toHaveBeenCalled();   

    // Add assertions.
    expect(userResult).toEqual({...mockUsers, tenant: tenant});
  });

  

  test('update should return error if tenant = null', async () => {
    // test data
    const mockUser = { 
      id: 1, 
      tenant: 2,
      createdOn: new Date(),
      username: 'JohnDoe', 
      password: 'password123', 
      email: 'sample@gmail.com', 
      firstName: 'John', 
      lastName: 'Doe', 
      mobile: '1234567890' 
    };

    // Call the method under test
    async function userResult() {
      await userDaoNoTenant.update(1, mockUser);
    }

    // Add assertions.
    await expect(userResult).rejects.toThrow("Tenant must be set before updating a user.");
  });
});