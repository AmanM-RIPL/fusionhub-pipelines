import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ChangeLogService } from "../change-log.service";
import { IChangeLog } from "../change-log.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { ColumnValue, InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { IUserRepository } from "../../user/user.repository";
import { IChangeLogRepository } from "../change-log.repository";
import { IProjectRepository } from "../../project/project.repository";

describe('Default -> ChangeLog -> ChangeLogService', () => {
  let changeLogService: ChangeLogService;
  let mockChangeLogRepository: DeepMockProxy<IChangeLogRepository>;
  let mockProjectRepository: DeepMockProxy<IProjectRepository>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

    beforeEach(() => {
        mockChangeLogRepository = mockDeep<IChangeLogRepository>();
        mockProjectRepository = mockDeep<IProjectRepository>();
        mockUserRepository = mockDeep<IUserRepository>();

        changeLogService = new ChangeLogService( mockChangeLogRepository, mockProjectRepository, 3);
      });


  afterEach(() => {
    jest.useRealTimers();
  });

  test('findById should call repository method with correct parameters', async () => {
    const mockChangeLog = { id: 1, entity: 'tenant' } as Selectable<IChangeLog>;
    mockChangeLogRepository.findById.mockResolvedValue(mockChangeLog);

    const result = await changeLogService.findById(1);

    expect(mockChangeLogRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockChangeLog);
  });

  test('findAll should call repository method with correct parameters', async () => {
    const mockChangeLogs = [{ id: 1, entity: 'tenant' }] as Selectable<IChangeLog>[];
    mockChangeLogRepository.findAll.mockResolvedValue(mockChangeLogs);

    const result = await changeLogService.findAll(10, 0);

    expect(mockChangeLogRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockChangeLogs);
  });

  test('create should call repository method with correct parameters', async () => {
    // mock date-time
    jest.useFakeTimers();
    jest.setSystemTime(new Date('2023-01-01T10:00:00Z'));
    
    const newChangeLog: InsertableEntity<IChangeLog> = {
      project: 201,
      entity: "employee",
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({
        user: 1,
        changeType: "create",
        description: "Approved By User",
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
      createdByUser: 1
    };

    const insertableChangeLog: Omit<InsertableEntity<IChangeLog>, "createdByUser" | "changeHistory"> = { project: newChangeLog.project, entity: newChangeLog.entity, entitySchema: newChangeLog.entitySchema};
    const createdChangeLog: Selectable<IChangeLog> = { 
      ...newChangeLog,
      id: 2,
      tenant: 1,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Approved By User",
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
      associatedApprovedEntity: null,
    };

    mockChangeLogRepository.create.mockResolvedValue(createdChangeLog);

    const result = await changeLogService.create(insertableChangeLog);

    expect(mockChangeLogRepository.create).toHaveBeenCalledWith(newChangeLog);
    expect(result).toEqual(createdChangeLog);
  });

});