import { InsertQueryBuilder, InsertResult, Kysely, Selectable, SelectQueryBuilder, UpdateQueryBuilder } from "kysely";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { UserDao } from "../dao/user.dao";
import { IUser } from "../user.model";

// Helper types for the chained query builder objects
type UsersSelectQueryBuilder = SelectQueryBuilder<IDatabase, 'public.fh_user', {}>;
type UsersInsertQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, InsertResult>;
type UsersInsertReturningAllQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, Selectable<IDatabase['public.fh_user']>>;
type UsersUpdateReturningAllQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.fh_user', 'public.fh_user', Selectable<IDatabase['public.fh_user']>>;
type UsersUpdateQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.fh_user', 'public.fh_user', {}>;

describe('Default -> User -> UserDao', () => {
  let mockKysely: DeepMockProxy<Kysely<IDatabase>>;
  let userDao: UserDao;
  let userDaoNoTenant: UserDao;
  const tenantId = 1;

  beforeEach(() => {
    mockKysely = mockDeep<Kysely<IDatabase>>();
    userDao = new UserDao(mockKysely, tenantId);
    userDaoNoTenant = new UserDao(mockKysely, null);
  });

  test('findById should call correct methods', async () => {
    // test data
    const mockUser: Selectable<IUser> = { 
      id: 1, 
      tenant: tenantId,
      createdOn: new Date(),
      username: 'JohnDoe', 
      password: 'password123', 
      email: 'sample@gmail.com', 
      firstName: 'John', 
      lastName: 'Doe', 
      mobile: '1234567890' 
    };

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<UsersSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockUser);

    // Call the method under test
    const userResult = await userDao.findById(1);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockUser: Selectable<IUser> = { 
      id: 1, 
      tenant: tenantId,
      createdOn: new Date(),
      username: 'JohnDoe', 
      password: 'password123', 
      email: 'sample@gmail.com', 
      firstName: 'John', 
      lastName: 'Doe', 
      mobile: '1234567890' 
    };

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<UsersSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockUser);

    // Call the method under test
    const userResult = await userDao.findById(1);

    // Add your assertions or method calls here
    expect(userResult).toEqual(mockUser);
  });

  test('findById should return error if tenant = null', async () => {
    // Call the method under test
    async function userResult() {
      await userDaoNoTenant.findById(1);
    }

    // Add your assertions or method calls here
    await expect(userResult).rejects.toThrow("Tenant must be set before accessing a user.");
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockUsers: Selectable<IUser>[] = [
      { 
        id: 1, 
        tenant: tenantId,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890' 
      },
      { 
        id: 2, 
        tenant: tenantId,
        createdOn: new Date(),
        username: 'JohnDoe1', 
        password: 'password1234', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890' 
      }
    ];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<UsersSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockUsers);


    // Call the method under test
    const userResult = await userDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.fh_user");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.limit).toHaveBeenCalledWith(10);
    expect(mockSelect.offset).toHaveBeenCalledWith(0);
    expect(mockSelect.execute).toHaveBeenCalled();
  });

  test('findAll should return correct value', async () => {
    // test data
    const mockUsers: Selectable<IUser>[] = [
      { 
        id: 1, 
        tenant: tenantId,
        createdOn: new Date(),
        username: 'JohnDoe', 
        password: 'password123', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890' 
      },
      { 
        id: 2, 
        tenant: tenantId,
        createdOn: new Date(),
        username: 'JohnDoe1', 
        password: 'password1234', 
        email: 'sample@gmail.com', 
        firstName: 'John', 
        lastName: 'Doe', 
        mobile: '1234567890' 
      }
    ];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<UsersSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockUsers);


    // Call the method under test
    const userResult = await userDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(userResult).toEqual(mockUsers);
  });

  test('findAll should return error if tenant = null', async () => {
    // Call the method under test
    async function userResult() {
      await userDaoNoTenant.findAll(10, 0);
    }

    // Add your assertions or method calls here
    await expect(userResult).rejects.toThrow("Tenant must be set before accessing a user.");
  });

  test('create should call correct methods', async () => {
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

    // Mock the Kysely methods to return the expected results
    const mockInsert = mockDeep<UsersInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<UsersInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce({...mockUser, tenant: tenantId});


    // Call the method under test
    const userResult = await userDao.create(mockUser);

    // Add your assertions or method calls here
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.fh_user");
    expect(mockInsert.values).toHaveBeenCalledWith({...mockUser, tenant: tenantId});
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
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

    // Mock the Kysely methods to return the expected results
    const mockInsert = mockDeep<UsersInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<UsersInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce({...mockUser, tenant: tenantId});


    // Call the method under test
    const userResult = await userDao.create(mockUser);

    // Add your assertions or method calls here
    expect(userResult).toEqual({...mockUser, tenant: tenantId});
  });

  test('create should return error if tenant = null', async () => {
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
      await userDaoNoTenant.create(mockUser);
    }

    // Add your assertions or method calls here
    await expect(userResult).rejects.toThrow("Tenant must be set before creating a user.");
  });

  test('update should call correct methods', async () => {
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

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<UsersUpdateQueryBuilder>();
    //@ts-ignore
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<UsersUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockUser);    

    // Call the method under test
    const userResult = await userDao.update(1, mockUser);

    // Add your assertions or method calls here
    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.fh_user");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockUser);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
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

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<UsersUpdateQueryBuilder>();
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<UsersUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);

    // tenantId is set to 1 in the UserDao constructor, so we need to ensure the mock reflects that
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce({...mockUser, tenant: tenantId});    

    // Call the method under test
    const userResult = await userDao.update(1, mockUser);

    // Add your assertions or method calls here
    expect(userResult).toEqual({...mockUser, tenant: tenantId});
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

    // Add your assertions or method calls here
    await expect(userResult).rejects.toThrow("Tenant must be set before updating a user.");
  });
});