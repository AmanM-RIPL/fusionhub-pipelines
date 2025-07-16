import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { PermissionService } from "../permission.service";
import { IPermission } from "../permission.model";
import { IBaseRepository } from "../../../common/repositories/base.repository";
import { IUserRepository } from "../../user/user.repository";
import { AccessPermission, FilterPermission, InsertableEntity } from "../../../common/types/entity";
import { Selectable } from "kysely";

describe('Default -> Permission -> PermissionService', () => {
  let permissionService: PermissionService;
  let mockPermissionRepository: DeepMockProxy<IBaseRepository<IPermission>>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  beforeEach(() => {
    mockPermissionRepository = mockDeep<IBaseRepository<IPermission>>();
    mockUserRepository = mockDeep<IUserRepository>();
    permissionService = new PermissionService(mockPermissionRepository, mockUserRepository);
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  test('create - should create a permission with valid data', async () => {
    const permissionData: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission } = {
      user: 1,
      entity: "WorkOrder",
      approval: [2, 3],
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };
    const permissionDataReturned: Selectable<IPermission> = {
      id: 1,
      tenant: 1,
      createdOn: new Date(),
      ...permissionData,
    };
    mockUserRepository.validateIds.mockResolvedValue(true);
    mockPermissionRepository.create.mockResolvedValue(permissionDataReturned);

    // Call the create method
    const result = await permissionService.create(permissionData);

    // Assertions
    expect(result).toEqual(permissionDataReturned);
  });

  test('create - should throw error if approval contains duplicate users', async () => {
    const permissionData: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission } = {
      user: 1,
      entity: "WorkOrder",
      approval: [2, 2], // Duplicate user
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };

    await expect(permissionService.create(permissionData)).rejects.toThrow("Approval array cannot contain duplicate users.");
  });

  test('create - should throw error if approval contains non-existent users', async () => {
    const permissionData: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission } = {
      user: 1,
      entity: "WorkOrder",
      approval: [2, 3], // Assuming user 3 does not exist
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };
    mockUserRepository.validateIds.mockResolvedValue(false);

    await expect(permissionService.create(permissionData)).rejects.toThrow("One or more users in the approval array do not exist.");
  });

  test('update - should update a permission with valid data', async () => {
    const updatedData: Omit<InsertableEntity<IPermission>, 'filter' | 'access' | 'user' | 'entity'> & { filter: FilterPermission, access: AccessPermission } = {
      approval: [2, 3],
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };

    const updatedPermission: Selectable<IPermission> = {
      id: 1,
      tenant: 1,
      createdOn: new Date(),
      user: 1,
      entity: "WorkOrder",
      ...updatedData,
    };
    mockUserRepository.validateIds.mockResolvedValue(true);
    mockPermissionRepository.update.mockResolvedValue(updatedPermission);

    const result = await permissionService.update(1, updatedData);

    expect(result).toEqual(updatedPermission);
  });

  test('update - should throw error if approval contains duplicate users', async () => {
    const updatedData: Omit<InsertableEntity<IPermission>, 'filter' | 'access' | 'user' | 'entity'> & { filter: FilterPermission, access: AccessPermission } = {
      approval: [2, 2], // Duplicate user
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };

    await expect(permissionService.update(1, updatedData)).rejects.toThrow("Approval array cannot contain duplicate users.");
  });
  
  test('update - should throw error if approval contains non-existent users', async () => {
    const updatedData: Omit<InsertableEntity<IPermission>, 'filter' | 'access' | 'user' | 'entity'> & { filter: FilterPermission, access: AccessPermission } = {
      approval: [2, 3], // Assuming user 3 does not exist
      filter: { project: [1, 2] },
      access: { create: true, read: true, update: false },
    };
    mockUserRepository.validateIds.mockResolvedValue(false);

    await expect(permissionService.update(1, updatedData)).rejects.toThrow("One or more users in the approval array do not exist.");
  });
});