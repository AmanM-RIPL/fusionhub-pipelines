import { Kysely, Selectable } from "kysely";
import { ChangeLogDao } from "../dao/change-log.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder } from "../../../common/types/test";

describe('Default -> ChangeLog -> ChangeLogDao', () => {
  let mockKysely: MockKysely;
  let changeLogDao: ChangeLogDao;
  let tenant = 2;
  let user = 2;

  beforeEach(() => {
    mockKysely = {
      selectFrom: jest.fn(),
      deleteFrom: jest.fn(),
      insertInto: jest.fn(),
      updateTable: jest.fn(),
    };

    changeLogDao = new ChangeLogDao(mockKysely  as unknown as Kysely<IDatabase>, tenant, user);
  });

  test('findById should call correct methods', async () => {
    const mockChangeLog = { id: 1, entity: 'tenant' };

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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockChangeLog);

    // Call the method under test
    const changeLogResult = await changeLogDao.findById(1);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.change_log");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockChangeLog = { id: 1, entity: 'tenant' };

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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockChangeLog);

    // Call the method under test
    const changeLogResult = await changeLogDao.findById(1);

    // Add assertions.
    expect(changeLogResult).toEqual(mockChangeLog);
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockChangeLogs = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

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
    mockQueryBuilder.execute.mockResolvedValueOnce(mockChangeLogs);

    // Call the method under test
    const changeLogResult = await changeLogDao.findAll(10, 0);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.change_log");
    expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
    expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
    expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
    expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
    expect(mockQueryBuilder.execute).toHaveBeenCalled();
  });

  test('create should call correct methods', async () => {
    // test data
    const mockChangeLogs: Selectable<IDatabase['public.change_log']> = {
      id: 1,
      tenant: 2,
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({}),
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), 
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      associatedApprovedEntity: 1
    };

    const mockChangeLogInsertable: InsertableEntity<IDatabase['public.change_log']> = {
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), 
            description: "",
            status: "approved"
          }
        ]
      }),
      createdByUser: 501,
      associatedApprovedEntity: 1
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockChangeLogs);

    // Call the method under test
    const changeLogResult = await changeLogDao.create(mockChangeLogInsertable);

    // Add assertions.
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.change_log");
    expect(mockInsert.values).toHaveBeenCalledWith({
      ...mockChangeLogInsertable,
      tenant: tenant // The DAO adds the tenant field
    });
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockInsert.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockChangeLogs: Selectable<IDatabase['public.change_log']> = {
      id: 1,
      tenant: 2,
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({}),
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), 
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      associatedApprovedEntity: 1
    };

    const mockChangeLogInsertable: InsertableEntity<IDatabase['public.change_log']> = {
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), 
            description: "",
            status: "approved"
          }
        ]
      }),
      createdByUser: 501,
      associatedApprovedEntity: 1
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockChangeLogs);

    // Call the method under test
    const changeLogResult = await changeLogDao.create(mockChangeLogInsertable);

    // Add assertions or method calls here
    expect(changeLogResult).toEqual(mockChangeLogs);
  });
});