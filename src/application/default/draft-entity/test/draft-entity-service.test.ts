import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { DraftEntityService } from "../draft-entity.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IDraftEntity } from "../draft-entity.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { ColumnValue, InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IUserRepository } from "../../user/user.repository";
import { IDraftEntityRepository } from "../draft-entity.repository";

describe('Default -> DraftEntity -> DraftEntityService', () => {
  let draftEntityService: DraftEntityService;
  let mockDraftEntityRepository: DeepMockProxy<IDraftEntityRepository>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  beforeEach(() => {
    mockDraftEntityRepository = mockDeep<IDraftEntityRepository>();
    mockUserRepository = mockDeep<IUserRepository>();
    draftEntityService = new DraftEntityService(mockDraftEntityRepository, 1);
  });

  afterEach(() => {
    jest.useRealTimers();
  });

  test('findById should call repository method with correct parameters', async () => {
    const mockDraftEntity = { id: 1, entity: 'tenant' } as Selectable<IDraftEntity>;
    mockDraftEntityRepository.findById.mockResolvedValue(mockDraftEntity);

    const result = await draftEntityService.findById(1);

    expect(mockDraftEntityRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockDraftEntity);
  });

  test('findAll should call repository method with correct parameters', async () => {
    const mockDraftEntitys = [{ id: 1, entity: 'tenant' }] as Selectable<IDraftEntity>[];
    mockDraftEntityRepository.findAll.mockResolvedValue(mockDraftEntitys);

    const result = await draftEntityService.findAll(10, 0);

    expect(mockDraftEntityRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockDraftEntitys);
  });

  test('create should call repository method with correct parameters', async () => {
    // mock date-time
    jest.useFakeTimers();
    jest.setSystemTime(new Date('2023-01-01T10:00:00Z'));
    
    const newDraftEntity: InsertableEntity<IDraftEntity> = {
      project: 201,
      entity: "employee",
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Approved by user",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 1,
            timestamp: new Date(),
            description: "Approved By User",
            status: "approved"
          }
        ]
      }),
      createdByUser: 1,
      nextApprovingUser: null
    };

    const insertableDraftEntity: Omit<InsertableEntity<IDraftEntity>, "createdByUser" | "nextApprovingUser" | "changeHistory"> = { project: newDraftEntity.project, entity: newDraftEntity.entity, entitySchema: newDraftEntity.entitySchema};
    const createdDraftEntity: Selectable<IDraftEntity> = { 
      ...newDraftEntity,
      id: 2,
      tenant: 1,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Approved by user",
        timestamp: new Date(),
        approvalHistory: [
          {
            user: 1,
            timestamp: new Date(),
            description: "Approved By User",
            status: "approved"
          }
        ]
      },
      entitySchema: {},
      createdOn: new Date(),
      createdByUser: 1,
      nextApprovingUser: null,
      associatedApprovedEntity: null,
    };

    mockDraftEntityRepository.create.mockResolvedValue(createdDraftEntity);

    const result = await draftEntityService.create(insertableDraftEntity);

    expect(mockDraftEntityRepository.create).toHaveBeenCalledWith(newDraftEntity);
    expect(result).toEqual(createdDraftEntity);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData = { entity: 'tenant' } as UpdateableEntity<IDraftEntity>;
    const updatedDraftEntity = { id: 1, entity: 'tenant' } as unknown as Selectable<IDraftEntity>;
    mockDraftEntityRepository.update.mockResolvedValue(updatedDraftEntity);

    const result = await draftEntityService.update(2, updatedData);

    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(2, updatedData);
    expect(result).toEqual(updatedDraftEntity);
  });

  test('findBy should call repository method with correct filters', async () => {
    const filters = { createdByUser: 201, entitySchema: 'employee' };
    const mockDraftEntities = [{ id: 3, entity: 'employee', project: 201 }] as Selectable<IDraftEntity>[];
    mockDraftEntityRepository.findBy.mockResolvedValue(mockDraftEntities);

    const result = await draftEntityService.findBy(filters);

    expect(mockDraftEntityRepository.findBy).toHaveBeenCalledWith(filters);
    expect(result).toEqual(mockDraftEntities);
  });

  test('approve should call repository update when not final approver', async () => {
    const id = 2;
    const approvalHierarchy = [1, 2];

    const mockDraftEntity = {
      project: 201,
      entity: "employee",
      entitySchema: "{}",
      associatedApprovedEntity: null,
      createdByUser: 2,
      nextApprovingUser: 1,
      changeHistory: { approvalHistory: [] }
    } as any as Selectable<IDraftEntity>;

    mockDraftEntityRepository.findById.mockResolvedValue(mockDraftEntity);
    mockDraftEntityRepository.update.mockResolvedValue({ ...mockDraftEntity, nextApprovingUser: 2 });

    const result = await draftEntityService.approve(id, approvalHierarchy);

    expect(mockDraftEntityRepository.findById).toHaveBeenCalledWith(id);
    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(id, expect.objectContaining({
      nextApprovingUser: 2
    }));
    expect(result).toBeUndefined();
  });

  test('approve should call repository.approve when final approver', async () => {
    const id = 2;
    const approvalHierarchy = [1];
    const user = 1;
    const changeDate = new Date();
    const mockDraftEntity: Selectable<IDraftEntity> = {
      id,
      tenant: 1,
      createdOn: changeDate,
      project: 201,
      entity: "employee",
      entitySchema: "{}",
      associatedApprovedEntity: null,
      createdByUser: 2,
      nextApprovingUser: user,
      changeHistory: { user: 1, description: '', changeType: 'create', timestamp: changeDate, approvalHistory: [] }
    };

    const approvedDraftEntity: Selectable<IDraftEntity> = {
      ...mockDraftEntity,
      changeHistory: {
        ...mockDraftEntity.changeHistory,
        approvalHistory: [
          ...mockDraftEntity.changeHistory.approvalHistory,
          {
            user,
            timestamp: new Date(),
            description: "Approved by user",
            status: "approved"
          }
        ]
      }
    };

    mockDraftEntityRepository.findById.mockResolvedValue(mockDraftEntity);
    mockDraftEntityRepository.approve.mockResolvedValue(approvedDraftEntity);

    const result = await draftEntityService.approve(id, approvalHierarchy);

    expect(mockDraftEntityRepository.findById).toHaveBeenCalledWith(id);
    expect(result?.changeHistory.approvalHistory).toHaveLength(1);
    expect(result?.changeHistory.approvalHistory[0].description).toEqual("Approved by user");
  });

 test('deleteById should call repository method with correct parameters', async () => {
  mockDraftEntityRepository.delete.mockResolvedValue(undefined);

  const result = await draftEntityService.delete(1);

  expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(1);
  expect(result).toBeUndefined();
 });

 test('should call repository delete method with correct id', async () => {
    const id = 1;
    mockDraftEntityRepository.delete.mockResolvedValue(undefined);

    await draftEntityService.delete(id);

    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(id);
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledTimes(1);
 });

 test('should handle repository delete method throwing error', async () => {
    const id = 1;
    const error = new Error('Database error');
    mockDraftEntityRepository.delete.mockRejectedValue(error);

    await expect(draftEntityService.delete(id)).rejects.toThrow('Database error');
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(id);
 });

 test('should not return anything when delete is successful', async () => {
    const id = 1;
    mockDraftEntityRepository.delete.mockResolvedValue(undefined);

    const result = await draftEntityService.delete(id);

    expect(result).toBeUndefined();
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(id);
 });

});