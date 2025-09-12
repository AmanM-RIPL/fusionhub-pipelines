import { Kysely, Selectable } from "kysely";
import { DraftEntityDao } from "../dao/draft-entity.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyDeleteQueryBuilder, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder, MockKyselyUpdateQueryBuilder } from "../../../common/types/test";

describe('Default -> DraftEntity -> DraftEntityDao', () => {
  let mockKysely: MockKysely;
  let draftEntityDao: DraftEntityDao;
  let tenant = 2;
  let user = 2;

  beforeEach(() => {
    mockKysely = {
      selectFrom: jest.fn(),
      deleteFrom: jest.fn(),
      insertInto: jest.fn(),
      updateTable: jest.fn(),
    };

    draftEntityDao = new DraftEntityDao(mockKysely  as unknown as Kysely<IDatabase>, tenant, user);
  });

  test('findById should call correct methods', async () => {
    const mockDraftEntity = { id: 1, entity: 'tenant' };

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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.findById(1);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.draft_entity");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockDraftEntity = { id: 1, entity: 'tenant' };

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
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.findById(1);

    // Add assertions.
    expect(draftEntityResult).toEqual(mockDraftEntity);
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockDraftEntitys = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

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
    mockQueryBuilder.execute.mockResolvedValueOnce(mockDraftEntitys);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.findAll(10, 0);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.draft_entity");
    expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
    expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
    expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
    expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
    expect(mockQueryBuilder.execute).toHaveBeenCalled();
  });

  test('create should call correct methods', async () => {
    // test data
    const mockDraftEntitys: Selectable<IDatabase['public.draft_entity']> = {
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
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      nextApprovingUser: 502,
      associatedApprovedEntity: 1
    };

    const mockDraftEntityInsertable: InsertableEntity<IDatabase['public.draft_entity']> = {
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
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      }),
      createdByUser: 501,
      nextApprovingUser: 502,
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockDraftEntitys);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.create(mockDraftEntityInsertable);

    // Add assertions or method calls here
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.draft_entity");
    expect(mockInsert.values).toHaveBeenCalledWith({
      ...mockDraftEntityInsertable,
      tenant: tenant // The DAO adds the tenant field
    });
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockInsert.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockDraftEntitys: Selectable<IDatabase['public.draft_entity']> = {
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
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      nextApprovingUser: 502,
      associatedApprovedEntity: 1
    };

    const mockDraftEntityInsertable: InsertableEntity<IDatabase['public.draft_entity']> = {
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
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      }),
      createdByUser: 501,
      nextApprovingUser: 502,
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
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockDraftEntitys);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.create(mockDraftEntityInsertable);

    // Add assertions.
    expect(draftEntityResult).toEqual(mockDraftEntitys);
  });

  test('update should call correct methods', async () => {
    // test data
    const mockDraftEntitys: Selectable<IDatabase['public.draft_entity']> = {
      id: 1,
      tenant: 2,
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({
        firstName: "Test",
        lastName: "TestLastName",
        email: "test@example.com"
      }),
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      nextApprovingUser: 502,
      associatedApprovedEntity: 1
    };

    const mockDraftEntityUpdateable: { [key: string]: unknown } = {
      firstName: "Test",
      lastName: "TestLastName",
      email: "test@example.com"
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
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockDraftEntitys);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.update(1, mockDraftEntityUpdateable);

    // Add assertions or method calls here
    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.draft_entity");
    expect(mockUpdate.set).toHaveBeenCalledWith({
      entitySchema: JSON.stringify(mockDraftEntityUpdateable)
    });
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockUpdate.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
    // test data
    const mockDraftEntitys: Selectable<IDatabase['public.draft_entity']> = {
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
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      },
      createdByUser: 501,
      nextApprovingUser: 502,
      associatedApprovedEntity: 1
    };

    const mockDraftEntityUpdateable:
      Omit<
        UpdateableEntity<IDatabase['public.draft_entity']>,
        "entitySchema" | "createdByUser" | "associatedApprovedEntity"
      > = {
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 2,
            timestamp: new Date(2024, 11, 12), // Fixed date constructor
            description: "",
            status: "approved"
          }
        ]
      }),
      nextApprovingUser: 502
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
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockDraftEntitys);

    // Call the method under test
    const draftEntityResult = await draftEntityDao.update(1, mockDraftEntityUpdateable);

    // Add assertions.
    expect(draftEntityResult).toEqual(mockDraftEntitys);
  });

  test('approve should insert into change_log and delete draft when nextApprovingUser is null', async () => {
    // Test data
    const mockDraftEntity: Selectable<IDatabase['public.draft_entity']> = {
      id: 1,
      tenant: 2,
      createdOn: new Date(),
      project: 202,
      entity: 'tenant',
      entitySchema: JSON.stringify({
        firstName: "Test",
        lastName: "TestLastName",
        email: "test@example.com"
      }),
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
      nextApprovingUser: null,
      associatedApprovedEntity: 1
    };

    // Mock UPDATE
    const mockUpdate = {
      set: jest.fn().mockReturnThis(),
      where: jest.fn().mockReturnThis(),
      returningAll: jest.fn().mockReturnThis(),
      executeTakeFirstOrThrow: jest.fn().mockResolvedValueOnce(mockDraftEntity),
    };
    mockKysely.updateTable.mockReturnValueOnce(mockUpdate);

    // Mock INSERT into change_log.
    const mockInsert = {
      values: jest.fn().mockReturnThis(),
      execute: jest.fn().mockResolvedValueOnce([{ id: 100 }]),
    };
    mockKysely.insertInto.mockReturnValueOnce(mockInsert);

    // Mock DELETE from draft_entity
    const mockDelete = {
      where: jest.fn().mockReturnThis(),
      execute: jest.fn().mockResolvedValueOnce([{ numDeletedRows: BigInt(1) }]),
    };
    mockKysely.deleteFrom.mockReturnValueOnce(mockDelete);

    const draftEntityResult = await draftEntityDao.approve(1, [501]);

    // Add  Assertions for INSERT
    expect(mockKysely.insertInto).toHaveBeenCalledWith('public.change_log');
    expect(mockInsert.values).toHaveBeenCalledWith(
      expect.objectContaining({
        tenant: mockDraftEntity.tenant,
        entity: mockDraftEntity.entity,
        project: mockDraftEntity.project,
        createdByUser: mockDraftEntity.createdByUser,
        changeHistory: JSON.stringify(mockDraftEntity.changeHistory),
      })
    );
    expect(mockInsert.execute).toHaveBeenCalled();

    // Assertions for DELETE
    expect(mockKysely.deleteFrom).toHaveBeenCalledWith('public.draft_entity');
    expect(mockDelete.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockDelete.where).toHaveBeenCalledWith("tenant", "=", mockDraftEntity.tenant);
    expect(mockDelete.execute).toHaveBeenCalled();

    // Final Result
    expect(draftEntityResult).toEqual(mockDraftEntity);
  });

  test('delete should call correct methods', async () => {
    const draftEntityId = 1;

    // Mock the select query builder
    const mockDraftEntity = { id: 1, entity: 'tenant', entitySchema: {}, nextApprovingUser: null, createdByUser: 2 };

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
    mockSelect.select.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);

    // Mock the delete query builder
    const mockDelete: MockKyselyDeleteQueryBuilder = {
      where: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      executeTakeFirst: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.deleteFrom.mockReturnValueOnce(mockDelete);
    mockDelete.where.mockReturnThis();
    mockDelete.execute.mockResolvedValueOnce([{ numDeletedRows: BigInt(1) }]);

    const result = await draftEntityDao.delete(draftEntityId);

    // Assertions
    expect(mockKysely.deleteFrom).toHaveBeenCalledWith("public.draft_entity");
    expect(mockDelete.where).toHaveBeenCalledWith("id", "=", draftEntityId);
    expect(mockDelete.where).toHaveBeenCalledWith("tenant", "=", tenant);
    expect(mockDelete.execute).toHaveBeenCalled();
    expect(result).toBeUndefined();
  });
  
});