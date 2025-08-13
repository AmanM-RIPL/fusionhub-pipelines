import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { DraftEntityService } from "../draft-entity.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IDraftEntity } from "../draft-entity.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { UpdateableEntity } from "../../../common/types/entity";
import { IUserRepository } from "../../user/user.repository";

describe('Default -> DraftEntity -> DraftEntityService', () => {
  let draftEntityService: DraftEntityService;
  let mockDraftEntityRepository: DeepMockProxy<IUnDeletableRepository<IDraftEntity>>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  beforeEach(() => {
    mockDraftEntityRepository = mockDeep<IUnDeletableRepository<IDraftEntity>>();
    mockUserRepository = mockDeep<IUserRepository>();
    draftEntityService = new DraftEntityService(mockDraftEntityRepository, mockUserRepository);
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
    const newDraftEntity = { 
      tenant: 1,
        project: 201,
        entity: "employee",
        entitySchema: JSON.stringify({}),
        changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: [
            {
            approvedBy: 2,
            approvedOn: new Date(),
            oldValue: "",
            newValue: "Initial"
            }
        ]
        }),
        data: "{}",
        createdByUser: 2,
        nextApprovingUser: 2,
        associatedApprovedEntity: null,
        parentDraftEntity: null
    } as (Insertable<IDraftEntity>);
    
    const insertableDraftEntity = { entity: newDraftEntity.entity,  } as Insertable<IDraftEntity>;
    const createdDraftEntity = { id: 2, entity: newDraftEntity.entity,  } as Selectable<IDraftEntity>;
    mockDraftEntityRepository.create.mockResolvedValue(createdDraftEntity);

    const result = await draftEntityService.create(newDraftEntity);

    expect(mockDraftEntityRepository.create).toHaveBeenCalledWith(insertableDraftEntity);
    expect(result).toEqual(createdDraftEntity);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData = { entity: 'Updated tenant' } as UpdateableEntity<IDraftEntity>;
    const updatedDraftEntity = { id: 1, ...updatedData } as Selectable<IDraftEntity>;
    mockDraftEntityRepository.update.mockResolvedValue(updatedDraftEntity);

    const result = await draftEntityService.update(1, updatedData);

    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(1, updatedData);
    expect(result).toEqual(updatedDraftEntity);
  });
});