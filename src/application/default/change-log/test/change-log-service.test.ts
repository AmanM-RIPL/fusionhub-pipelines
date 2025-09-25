import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ChangeLogService } from "../change-log.service";
import { IChangeLog } from "../change-log.model";
import { Selectable } from "kysely";
import { InsertableEntity } from "../../../common/types/entity";
import { IUserRepository } from "../../user/user.repository";
import { IChangeLogRepository } from "../change-log.repository";
import { IProjectRepository } from "../../project/project.repository";
import { IProject } from "../../project/project.model";
import path from "path";
import Database from "better-sqlite3";
import { SQLiteSync } from "../entities/_main/sqlite.factory";

jest.mock('../entities/_main/sqlite.factory', () => ({
  SQLiteSync: jest.fn().mockImplementation(() => ({
    syncSqlite: jest.fn(),
  })),
}));

jest.mock("better-sqlite3", () => {
  return jest.fn().mockImplementation(() => ({
    transaction: jest.fn(() => jest.fn()),
    close: jest.fn(),
    prepare: jest.fn(),
  }));
});

describe("Default -> ChangeLog -> ChangeLogService", () => {
  let changeLogService: ChangeLogService;
  let mockChangeLogRepository: DeepMockProxy<IChangeLogRepository>;
  let mockProjectRepository: DeepMockProxy<IProjectRepository>;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  const MockedDatabase = Database as jest.MockedClass<typeof Database>;
  const MockedSQLiteSync = SQLiteSync as jest.MockedClass<typeof SQLiteSync>;
  const mockUser = 1;

  beforeEach(() => {
    mockChangeLogRepository = mockDeep<IChangeLogRepository>();
    mockProjectRepository = mockDeep<IProjectRepository>();
    mockUserRepository = mockDeep<IUserRepository>();

    changeLogService = new ChangeLogService(
      mockChangeLogRepository,
      mockProjectRepository,
      mockUser
    );
  });

  afterEach(() => {
    jest.useRealTimers();
    jest.clearAllMocks();
  });

  test("findById should call repository method with correct parameters", async () => {
    const mockChangeLog = { id: 1, entity: "tenant" } as Selectable<IChangeLog>;
    mockChangeLogRepository.findById.mockResolvedValue(mockChangeLog);

    const result = await changeLogService.findById(1);

    expect(mockChangeLogRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockChangeLog);
  });

  test("findAll should call repository method with correct parameters", async () => {
    const mockChangeLogs = [{ id: 1, entity: "tenant" }] as Selectable<IChangeLog>[];
    mockChangeLogRepository.findAll.mockResolvedValue(mockChangeLogs);

    const result = await changeLogService.findAll(10, 0);

    expect(mockChangeLogRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockChangeLogs);
  });

  test("create should call repository method with correct parameters", async () => {
     jest.useFakeTimers();
    jest.setSystemTime(new Date("2023-01-01T10:00:00Z"));

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
            status: "approved",
          },
        ],
      }),
      createdByUser: 1,
    };

    const insertableChangeLog: Omit<
      InsertableEntity<IChangeLog>,
      "createdByUser" | "changeHistory"
    > = {
      project: newChangeLog.project,
      entity: newChangeLog.entity,
      entitySchema: newChangeLog.entitySchema,
    };

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
            status: "approved",
          },
        ],
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

  test("should fetch project, fetch change logs, and sync into SQLite", async () => {
    const projectId = 1;
    // Mock project returned from Postgres
    const mockProject: Selectable<IProject> = {
      id: projectId,
      tenant: 1,
      createdOn: new Date(),
      projectName: "Test Project",
      description: "Test Description",
      startDate: new Date(),
      endDate: null,
      status: "active",
      customerName: "Test Customer",
      customerEmail: "test@example.com",
      customerMobile: null,
      lastChangeLogId: 10,
      isBlocked: false,
    };
    mockProjectRepository.findById.mockResolvedValue(mockProject);

    // Mock change logs returned from Postgres
    const newChangeLogs: Selectable<IChangeLog>[] = [
      {
        id: 1,
        tenant: 1,
        project: projectId,
        entity: "employee",
        entitySchema: {},
        changeHistory: { user: 1, changeType: "create", description: "Approved By User", timestamp: new Date(), approvalHistory: [] },
        createdOn: new Date(),
        createdByUser: 1,
        associatedApprovedEntity: null,
      },
      {
        id: 2,
        tenant: 1,
        project: projectId,
        entity: "employee",
        entitySchema: {},
        changeHistory: { user: 2, changeType: "update", description: "Updated By User", timestamp: new Date(), approvalHistory: [] },
        createdOn: new Date(),
        createdByUser: 2,
        associatedApprovedEntity: 100,
      },
    ];
    mockChangeLogRepository.findAllWithChangeLogGreaterThan.mockResolvedValue(newChangeLogs);

    // Mock SQLite DB
    const mockDbInstance = { close: jest.fn() };
    MockedDatabase.mockImplementation(() => mockDbInstance as any);

    // Mock SQLiteSync
    const mockSqliteSync = { syncSqlite: jest.fn().mockResolvedValue(undefined) };
    (SQLiteSync as jest.Mock).mockImplementation(() => mockSqliteSync as any);

    await changeLogService.changeLogSync(projectId);

    // Assertions
    expect(mockProjectRepository.findById).toHaveBeenCalledWith(projectId);
    expect(mockChangeLogRepository.findAllWithChangeLogGreaterThan).toHaveBeenCalledWith(
      projectId,
      mockProject.lastChangeLogId,
      50,
      0
    );

    expect(mockSqliteSync.syncSqlite).toHaveBeenCalledWith(newChangeLogs);

    const dbFilePath = path.resolve(`C:/code/fhapi_service/src/infrastructure/sqlite/project_${projectId}.db`);
    expect(MockedDatabase).toHaveBeenCalledWith(dbFilePath);
    expect(mockDbInstance.close).toHaveBeenCalled();
  });

  test("should stop if project not found", async () => {
    mockProjectRepository.findById.mockResolvedValue(undefined);

    await expect(changeLogService.changeLogSync(999)).rejects.toThrow(
      "Project 999 not found in Postgres."
    );
    expect(mockChangeLogRepository.findAllWithChangeLogGreaterThan).not.toHaveBeenCalled();
  });

  test("should log and return if no new change logs", async () => {
    const projectId = 201;
    const mockProject: Selectable<IProject> = {
      id: projectId,
      tenant: 1,
      createdOn: new Date(),
      projectName: "Test Project",
      description: "Test Description",
      startDate: new Date(),
      endDate: null,
      status: "active",
      customerName: "Test Customer",
      customerEmail: "test@example.com",
      customerMobile: null,
      lastChangeLogId: 10,
      isBlocked: false,
    };
    mockProjectRepository.findById.mockResolvedValue(mockProject);
    mockChangeLogRepository.findAllWithChangeLogGreaterThan.mockResolvedValue([]);

    const consoleSpy = jest.spyOn(console, "log").mockImplementation(() => { });

    await changeLogService.changeLogSync(projectId);

    expect(consoleSpy).toHaveBeenCalledWith(`No new change logs for project ${projectId}`);
    expect(SQLiteSync).not.toHaveBeenCalled();

    consoleSpy.mockRestore();
  });
  
});
