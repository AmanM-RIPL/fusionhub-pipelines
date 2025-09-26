import { Kysely, Selectable } from "kysely";
import { PermissionDao } from "../dao/permission.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyDeleteQueryBuilder, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder, MockKyselyUpdateQueryBuilder } from "../../../common/types/test";

describe('Default -> Permission -> PermissionDao', () => {
    let mockKysely: MockKysely;
    let permissionDao: PermissionDao;
    let tenant = 2;
    let user = 2;

    beforeEach(() => {
        mockKysely = {
            selectFrom: jest.fn(),
            deleteFrom: jest.fn(),
            insertInto: jest.fn(),
            updateTable: jest.fn(),
        };

        permissionDao = new PermissionDao(mockKysely as unknown as Kysely<IDatabase>, tenant);
    });

    test('findById should call correct methods', async () => {
        const mockPermission = { id: 1, entity: 'tenant' };

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
        mockSelect.executeTakeFirst.mockResolvedValueOnce(mockPermission);

        // Call the method under test
        const permissionResult = await permissionDao.findById(1);

        // Add assertions.
        expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.permission");
        expect(mockSelect.selectAll).toHaveBeenCalled();
        expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
        expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
    });

    test('findById should return correct value', async () => {
        // test data
        const mockPermission = { id: 1, entity: 'tenant' };

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
        mockSelect.executeTakeFirst.mockResolvedValueOnce(mockPermission);

        // Call the method under test
        const permissionResult = await permissionDao.findById(1);

        // Add assertions.
        expect(permissionResult).toEqual(mockPermission);
    });

    test('findAll should call correct methods', async () => {
        // test data
        const mockPermissions = [{ id: 1, entity: 'Create' }, { id: 2, entity: 'Create' }];

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
        mockQueryBuilder.execute.mockResolvedValueOnce(mockPermissions);

        // Call the method under test
        const permissionResult = await permissionDao.findAll(10, 0);

        // Add assertions.
        expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.permission");
        expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
        expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
        expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
        expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
        expect(mockQueryBuilder.execute).toHaveBeenCalled();
    });

    test('create should call correct methods', async () => {
        // test data
        const mockPermissions: Selectable<IDatabase['public.permission']> = {
            id: 2,
            tenant: 1,
            createdOn: new Date(),
            user: 1,
            project: 201,
            entity: "employee",
            permissionType: 'CREATE',
            approval: [501, 502],
            filter: {
                project: [1, 2, 3],
                subProject: [1, 2, 3],
                businessEntityBranch: [1, 2, 3],
                financialYear: [1, 2, 3],
            },
            access: {
                create: true,
                read: true,
                update: true
            }
        };

        const mockPermissionInsertable: InsertableEntity<IDatabase['public.permission']> = {
            createdOn: new Date(),
            user: 1,
            project: 201,
            entity: "employee",
            permissionType: 'CREATE',
            approval: [501, 502],
            filter: ""
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
        mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockPermissions);

        // Call the method under test
        const permissionResult = await permissionDao.create(mockPermissionInsertable);

        // Add assertions.
        expect(mockKysely.insertInto).toHaveBeenCalledWith("public.permission");
        expect(mockInsert.values).toHaveBeenCalledWith({
            ...mockPermissionInsertable,
            tenant: tenant // The DAO adds the tenant field
        });
        expect(mockInsert.returningAll).toHaveBeenCalled();
        expect(mockInsert.executeTakeFirstOrThrow).toHaveBeenCalled();
    });

    test('create should return correct value', async () => {
        // test data
        const mockPermissions: Selectable<IDatabase['public.permission']> = {
            id: 2,
            tenant: 1,
            createdOn: new Date(),
            user: 1,
            project: 201,
            entity: "employee",
            permissionType: 'CREATE',
            approval: [501, 502],
            filter: {
                project: [1, 2, 3],
                subProject: [1, 2, 3],
                businessEntityBranch: [1, 2, 3],
                financialYear: [1, 2, 3],
            },
            access: {
                create: true,
                read: true,
                update: true
            }
        };

        const mockPermissionInsertable: InsertableEntity<IDatabase['public.permission']> = {
            createdOn: new Date(),
            user: 1,
            project: 201,
            entity: "employee",
            permissionType: 'CREATE',
            approval: [501, 502],
            filter: ""
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
        mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockPermissions);

        // Call the method under test
        const permissionResult = await permissionDao.create(mockPermissionInsertable);

        // Add assertions.
        expect(permissionResult).toEqual(mockPermissions);
    });

    test('update should call correct methods', async () => {
    // test data
    const mockPermissions: Selectable<IDatabase['public.permission']> = {
        id: 2,
        tenant: 1,
        createdOn: new Date(),
        user: 1,
        project: 201,
        entity: "employee",
        permissionType: 'CREATE',
        approval: [501, 502],
        filter: {
            project: [1, 2, 3],
            subProject: [1, 2, 3],
            businessEntityBranch: [1, 2, 3],
            financialYear: [1, 2, 3],
        },
        access: {
            create: true,
            read: true,
            update: true
        }
    };

    const mockPermissionUpdateable:
        Omit<
            UpdateableEntity<IDatabase['public.permission']>,
            "entitySchema" | "createdByUser" | "associatedApprovedEntity"
        > = {
            entity: "employee",
            permissionType: 'CREATE',
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
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockPermissions);

    // Call the method under test
    const permissionResult = await permissionDao.update(1, mockPermissionUpdateable);

    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.permission");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockPermissionUpdateable);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockUpdate.executeTakeFirst).toHaveBeenCalled();
});

    test('update should return correct value', async () => {
        // test data
        const mockPermissions: Selectable<IDatabase['public.permission']> = {
            id: 2,
            tenant: 1,
            createdOn: new Date(),
            user: 1,
            project: 201,
            entity: "employee",
            permissionType: 'CREATE',
            approval: [501, 502],
            filter: {
                project: [1, 2, 3],
                subProject: [1, 2, 3],
                businessEntityBranch: [1, 2, 3],
                financialYear: [1, 2, 3],
            },
            access: {
                create: true,
                read: true,
                update: true
            }
        };

        const mockPermissionUpdateable:
            Omit<
                UpdateableEntity<IDatabase['public.permission']>,
                "entitySchema" | "createdByUser" | "associatedApprovedEntity"
            > = {
            entity: "employee",
            permissionType: 'CREATE',
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
        mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockPermissions);

        // Call the method under test
        const permissionResult = await permissionDao.update(1, mockPermissionUpdateable);

        // Add assertions.
        expect(permissionResult).toEqual(mockPermissions);
    });

    test('delete should call correct methods', async () => {
        const permissionId = 1;

        // Mock the select query builder
        const mockPermission = { id: 1, entity: 'tenant', entitySchema: {}, nextApprovingUser: null, createdByUser: 2 };

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
        mockSelect.executeTakeFirst.mockResolvedValueOnce(mockPermission);

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

        const result = await permissionDao.delete(permissionId);

        // Assertions
        expect(mockKysely.deleteFrom).toHaveBeenCalledWith("public.permission");
        expect(mockDelete.where).toHaveBeenCalledWith("id", "=", permissionId);
        expect(mockDelete.where).toHaveBeenCalledWith("tenant", "=", tenant);
        expect(mockDelete.execute).toHaveBeenCalled();
        expect(result).toBeUndefined();
    });
});