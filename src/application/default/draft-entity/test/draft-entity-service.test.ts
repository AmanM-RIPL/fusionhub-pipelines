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
    const newDraftEntity
    : Omit<InsertableEntity<IDraftEntity>, "createdByUser" | "nextApprovingUser" | "changeHistory" | "data"> & {
    data: ColumnValue;
  } = {
      project: 201,
      entity: "employee",
      entitySchema: JSON.stringify({}),
      data: {},
      associatedApprovedEntity: null,
      parentDraftEntity: null
    }
    
    const insertableDraftEntity = { entity: newDraftEntity.entity,  } as Insertable<IDraftEntity>;
    const createdDraftEntity = { id: 2, entity: newDraftEntity.entity,  } as Selectable<IDraftEntity>;
    mockDraftEntityRepository.create.mockResolvedValue(createdDraftEntity);

    const result = await draftEntityService.create(newDraftEntity);

    expect(mockDraftEntityRepository.create).toHaveBeenCalledWith(insertableDraftEntity);
    expect(result).toEqual(createdDraftEntity);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData: { data: ColumnValue; } = { data: { } };
    const updatedDraftEntity = { id: 1, ...updatedData } as Selectable<IDraftEntity>;
    mockDraftEntityRepository.update.mockResolvedValue(updatedDraftEntity);

    const result = await draftEntityService.update(1, updatedData);

    expect(mockDraftEntityRepository.update).toHaveBeenCalledWith(1, updatedData);
    expect(result).toEqual(updatedDraftEntity);
  });
});