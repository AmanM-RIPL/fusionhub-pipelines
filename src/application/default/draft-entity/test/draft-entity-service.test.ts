import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { DraftEntityService } from "../draft-entity.service";
import { IDraftEntityRepository } from "../draft-entity.repository";
import { InsertableEntity } from "../../../common/types/entity";
import { IDraftEntity } from "../draft-entity.model";
import { Selectable } from "kysely";

describe('Default -> DraftEntity -> DraftEntityService', () => {
  let draftEntityService: DraftEntityService;
  let mockDraftEntityRepository: DeepMockProxy<IDraftEntityRepository>;
  const userId = 1;
  const mockedDate = new Date('2023-10-26T12:00:00.000Z');

  beforeEach(() => {
    jest.useFakeTimers();
    jest.setSystemTime(mockedDate);
    mockDraftEntityRepository = mockDeep<IDraftEntityRepository>();
    draftEntityService = new DraftEntityService(mockDraftEntityRepository, userId);
  });

  afterEach(() => {
    jest.clearAllMocks();
    jest.useRealTimers();
  });

  test('create - should create a new draft entity with correct data', async () => {
    // Arrange
    const entityData = {
      entitySchema: "WorkOrder",
      associatedApprovedEntity: null,
      parentDraftEntity: null,
      data: { key: "value" },
      nextApprovingUser: null
    };
    const expectedEntity: InsertableEntity<IDraftEntity>  = {
      ...entityData,
      createdByUser: userId,
      nextApprovingUser: null,
      changeHistory: JSON.stringify({
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: []
      }),
      data: JSON.stringify(entityData.data)
    };

    const entityReturnedByRepository: Selectable<IDraftEntity> = {
      ...expectedEntity,
      id: 1,
      tenant: 1,
      parentDraftEntity: null,
      associatedApprovedEntity: null,
      nextApprovingUser: null,
      data: JSON.parse(expectedEntity.data),
      changeHistory: JSON.parse(expectedEntity.changeHistory),
    };

    mockDraftEntityRepository.create.mockResolvedValue(entityReturnedByRepository);

    // Act
    const result = await draftEntityService.create(entityData);

    // Assert
    expect(mockDraftEntityRepository.create).toHaveBeenCalledWith(expectedEntity);
    expect(result).toEqual(entityReturnedByRepository);
  });

  test('update - should update the draft entity data', async () => {
    // Arrange
    const entityId = 1;
    const updatedData = { key: "newValue" };
    const expectedUpdate = { data: JSON.stringify(updatedData) };

    const updatedEntity: Selectable<IDraftEntity> = {
      id: entityId,
      tenant: 1,
      createdByUser: userId,
      nextApprovingUser: null,
      entitySchema: "WorkOrder",
      associatedApprovedEntity: null,
      parentDraftEntity: null,
      changeHistory: {
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: []
      },
      data: updatedData
    };

    mockDraftEntityRepository.update.mockResolvedValue(updatedEntity);

    // Act
    const result = await draftEntityService.update(entityId, { data: updatedData });

    // Assert
    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(entityId, expectedUpdate);
    expect(result).toEqual(updatedEntity);
  });

  test('delete - should delete the draft entity and its children', async () => {
    // Arrange
    const entityId = 1;
    const childEntities: Selectable<IDraftEntity>[] = [
      { 
        id: 2, 
        tenant: 1, 
        createdByUser: userId, 
        nextApprovingUser: null, 
        entitySchema: "WorkOrder", 
        associatedApprovedEntity: null, 
        parentDraftEntity: entityId, 
        changeHistory: {
          user: userId,
          changeType: "create",
          description: "",
          timestamp: mockedDate,
          approvalHistory: []
        }, 
        data: {} 
      },
      { 
        id: 3, 
        tenant: 1, 
        createdByUser: userId, 
        nextApprovingUser: null, 
        entitySchema: "WorkOrder", 
        associatedApprovedEntity: null, 
        parentDraftEntity: entityId, 
        changeHistory: {
          user: userId,
          changeType: "create",
          description: "",
          timestamp: mockedDate,
          approvalHistory: []
        }, 
        data: {} 
      }
    ];

    mockDraftEntityRepository.findBy.mockResolvedValue(childEntities);
    mockDraftEntityRepository.delete.mockResolvedValue();

    // Act
    await draftEntityService.delete(entityId);

    // Assert
    expect(mockDraftEntityRepository.findBy).toHaveBeenCalledWith({ parentDraftEntity: entityId });
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(2);
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(3);
    expect(mockDraftEntityRepository.delete).toHaveBeenCalledWith(entityId);
  });

  test('approve - should update the next approving user and change history', async () => {
    // Arrange
    const entityId = 1;
    const approvalHierarchy = [1, 2, 3];
    const draftEntity: Selectable<IDraftEntity> = {
      id: entityId,
      tenant: 1,
      createdByUser: userId,
      nextApprovingUser: 1,
      entitySchema: "WorkOrder",
      associatedApprovedEntity: null,
      parentDraftEntity: null,
      changeHistory: {
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: []
      },
      data: {}
    };

    mockDraftEntityRepository.findById.mockResolvedValue(draftEntity);

    // Act
    const result = await draftEntityService.approve(entityId, approvalHierarchy);

    // Assert
    expect(mockDraftEntityRepository.findById).toHaveBeenCalledWith(entityId);
    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(entityId, {
      nextApprovingUser: 2,
      changeHistory: JSON.stringify({
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: [
          {
            user: userId,
            timestamp: mockedDate,
            description: "Approved by user",
            status: "approved"
          }
        ]
      })
    });

    expect(result).toBeUndefined();
  });

  test('approve - should approve the draft entity if the user is the last in the approval hierarchy', async () => {
    // Arrange
    const entityId = 1;
    const approvalHierarchy = [1];
    const draftEntity: Selectable<IDraftEntity> = {
      id: entityId,
      tenant: 1,
      createdByUser: userId,
      nextApprovingUser: 1,
      entitySchema: "WorkOrder",
      associatedApprovedEntity: null,
      parentDraftEntity: null,
      changeHistory: {
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: []
      },
      data: {}
    };
    mockDraftEntityRepository.findById.mockResolvedValue(draftEntity);
    mockDraftEntityRepository.update.mockResolvedValue({
      ...draftEntity,
      nextApprovingUser: null,
      changeHistory: {
        ...draftEntity.changeHistory,
        approvalHistory: [
          {
            user: userId,
            timestamp: mockedDate,
            description: "Approved by user",
            status: "approved"
          }
        ]
      }
    });

    // Act
    const result = await draftEntityService.approve(entityId, approvalHierarchy);

    // Assert
    expect(mockDraftEntityRepository.findById).toHaveBeenCalledWith(entityId);
    expect(result).toEqual({
      ...draftEntity,
      nextApprovingUser: 1,
      changeHistory: {
        ...draftEntity.changeHistory,
        approvalHistory: [
          {
            user: userId,
            timestamp: mockedDate,
            description: "Approved by user",
            status: "approved"
          }
        ]
      }
    });
  });
  
  test('approve - should throw an error if the user is not the next approving user', async () => {
    // Arrange
    const entityId = 1;
    const approvalHierarchy = [2, 3];
    const draftEntity: Selectable<IDraftEntity> = {
      id: entityId,
      tenant: 1,
      createdByUser: userId,
      nextApprovingUser: 2,
      entitySchema: "WorkOrder",
      associatedApprovedEntity: null,
      parentDraftEntity: null,
      changeHistory: {
        user: userId,
        changeType: "create",
        description: "",
        timestamp: mockedDate,
        approvalHistory: []
      },
      data: {}
    };

    mockDraftEntityRepository.findById.mockResolvedValue(draftEntity);

    // Act & Assert
    await expect(draftEntityService.approve(entityId, approvalHierarchy)).rejects.toThrowError(`User ${userId} is not the next approving user for draft entity with id ${entityId}.`);
  });
  
  test('approve - should throw an error if the draft entity is not found', async () => {
    // Arrange
    const entityId = 1;
    const approvalHierarchy = [1, 2, 3];

    mockDraftEntityRepository.findById.mockResolvedValue(undefined);

    // Act & Assert
    await expect(draftEntityService.approve(entityId, approvalHierarchy)).rejects.toThrowError(`Draft entity with id ${entityId} not found.`);
  });
});