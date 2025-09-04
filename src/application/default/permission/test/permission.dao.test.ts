import { Insertable, InsertQueryBuilder, InsertResult, Kysely, Selectable, SelectQueryBuilder, DeleteQueryBuilder, DeleteResult, UpdateQueryBuilder } from "kysely";
import { PermissionDao } from "../dao/permission.dao";
import { mockDeep, DeepMockProxy } from 'jest-mock-extended';
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";

type PermissionSelectQueryBuilder = SelectQueryBuilder<IDatabase, 'public.permission', {}>;
type PermissionDeleteQueryBuilder = DeleteQueryBuilder<IDatabase, 'public.permission', DeleteResult>;
type PermissionInsertQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, InsertResult>;
type PermissionInsertReturningAllQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, Selectable<IDatabase['public.permission']>>;
type PermissionUpdateReturningAllQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.permission', 'public.permission', Selectable<IDatabase['public.permission']>>;
type PermissionUpdateQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.permission', 'public.permission', {}>;

describe('Default -> Permission -> PermissionDao', () => {
    let mockKysely: DeepMockProxy<Kysely<IDatabase>>;
    let permissionDao: PermissionDao;
    let tenant = 1;

    beforeEach(() => {
        mockKysely = mockDeep<Kysely<IDatabase>>();
        permissionDao = new PermissionDao(mockKysely, tenant);
    });

    test('findById should call correct methods', async () => {
        const mockCreatedPermission = { id: 1, entity: 'tenant' };

        const mockSelect = mockDeep<PermissionSelectQueryBuilder>();
        mockKysely.selectFrom.mockReturnValue(mockSelect);
        mockSelect.selectAll.mockReturnThis();
        mockSelect.where.mockReturnThis();
        mockSelect.executeTakeFirst.mockResolvedValueOnce(mockCreatedPermission);

        const permissionResult = await permissionDao.findById(2);

        expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.permission");
        expect(mockSelect.selectAll).toHaveBeenCalled();
        expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 2);
        expect(mockSelect.where).toHaveBeenCalledWith("tenant", "=", tenant);
        expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
        expect(permissionResult).toEqual(mockCreatedPermission); 
    });

    test('findById should throw error when tenant is null', async () => {
        permissionDao = new PermissionDao(mockKysely, null);

        await expect(permissionDao.findById(2)).rejects.toThrow("Tenant must be set before accessing an permission.");
    });

    test('findAll should return correct values', async () => {
        const mockCreatedPermission = { id: 1, entity: 'tenant' };
        const mockQueryBuilder = mockDeep<PermissionSelectQueryBuilder>();
        mockKysely.selectFrom.mockReturnValue(mockQueryBuilder);
        mockQueryBuilder.where.mockReturnThis();
        mockQueryBuilder.selectAll.mockReturnThis();
        mockQueryBuilder.limit.mockReturnThis();
        mockQueryBuilder.offset.mockReturnThis();
        mockQueryBuilder.execute.mockResolvedValueOnce([mockCreatedPermission]);

        const result = await permissionDao.findAll(10, 0);

        expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.permission");
        expect(mockQueryBuilder.where).toHaveBeenCalledWith("tenant", "=", tenant);
        expect(mockQueryBuilder.selectAll).toHaveBeenCalled();
        expect(mockQueryBuilder.limit).toHaveBeenCalledWith(10);
        expect(mockQueryBuilder.offset).toHaveBeenCalledWith(0);
        expect(result).toEqual([mockCreatedPermission]);
    });

    test('findAll should throw error when tenant is null', async () => {
        permissionDao = new PermissionDao(mockKysely, null);

        await expect(permissionDao.findAll(10, 0)).rejects.toThrow(
            "Tenant must be set before accessing an permission."
        );
    });

    test('create should insert correctly', async () => {
        const mockPermission: Selectable<IDatabase['public.permission']> = {
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

        const mockInsert = mockDeep<PermissionInsertQueryBuilder>();
        mockKysely.insertInto.mockReturnValue(mockInsert);
        mockInsert.values.mockReturnThis();
        const mockReturningAll = mockDeep<PermissionInsertReturningAllQueryBuilder>();
        mockInsert.returningAll.mockReturnValue(mockReturningAll);
        mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockPermission);

        const result = await permissionDao.create(mockPermissionInsertable);

        expect(mockKysely.insertInto).toHaveBeenCalledWith("public.permission");
        expect(mockInsert.values).toHaveBeenCalledWith({
            ...mockPermissionInsertable,
            tenant: tenant
        });
        expect(mockInsert.returningAll).toHaveBeenCalled();
        expect(result).toEqual(mockPermission);
    });

    test('update should modify correctly', async () => {
        const mockPermission: Selectable<IDatabase['public.permission']> = {
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

        const mockPermissionUpdateable: { [key: string]: unknown } = {
            entity: "employee",
            permissionType: 'CREATE',
        };

        const mockUpdate = mockDeep<PermissionUpdateQueryBuilder>();
        mockKysely.updateTable.mockReturnValue(mockUpdate);
        mockUpdate.set.mockReturnThis();
        mockUpdate.where.mockReturnThis();
        const mockReturningAll = mockDeep<PermissionUpdateReturningAllQueryBuilder>();
        mockUpdate.returningAll.mockReturnValue(mockReturningAll);
        mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockPermission);

        const result = await permissionDao.update(2, mockPermissionUpdateable);

        expect(mockKysely.updateTable).toHaveBeenCalledWith("public.permission");
        expect(mockUpdate.set).toHaveBeenCalledWith(mockPermissionUpdateable);
        expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 2);
        expect(mockUpdate.where).toHaveBeenCalledWith("tenant", "=", tenant);
        expect(result).toEqual(mockPermission);
    });

    test('delete should call correct methods', async () => {
        const permissionId = 1;
        const mockDraftEntity = {
            id: 1, entity: 'tenant', entitySchema: {}, filter: {
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

        const mockSelect = mockDeep<PermissionSelectQueryBuilder>();
        mockKysely.selectFrom.mockReturnValue(mockSelect);
        mockSelect.select.mockReturnThis();
        mockSelect.where.mockReturnThis();
        mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);

        // Mock the delete query builder
        const mockDelete = mockDeep<PermissionDeleteQueryBuilder>();
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
