import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ChangeLogService } from "../change-log.service";
import changeLogRoutes from "../change-log.route";
import { Insertable, Selectable } from "kysely";
import { IChangeLog } from "../change-log.model";
import { UpdateableEntity, ChangeHistory, ColumnValue, InsertableEntity, ChangeHistoryPayload } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> ChangeLog -> ChangeLogRoute', () => {
  let fastify: FastifyInstance;
  let changeLogService: DeepMockProxy<ChangeLogService>;

  beforeAll(async () => {
    fastify = Fastify({ logger: false }) as unknown as FastifyInstance;
    changeLogService = mockDeep<ChangeLogService>();

    // Mock the changeLogService methods
    fastify.decorateRequest('changeLogService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<ChangeLogService>('changeLogService', changeLogService);
    });

    fastify.setErrorHandler(async (error: Error, request, reply) => {
      let statusCode = 500;
      let errorMessage = 'Internal Server Error';

      if (error instanceof CustomError) {
        statusCode = error.statusCode;
        errorMessage = error.message;
      }

      reply.code(statusCode).send({ error: errorMessage });
    });
    await changeLogRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should register changeLog routes', async () => {
    const routes = fastify.printRoutes();
    expect(routes).toContain(':changeLogId');
    expect(routes).toContain('changeLog');
  });

  test('should handle get changeLog by id', async () => {
    const mockChangeLog = {
      id: 13,
      tenant: 1,
      project: 2,
      entity: "employee",
      createdByUser: 2,
      entitySchema: {},
      associatedApprovedEntity: null,
      createdOn: new Date(),
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: []
      } as ChangeHistory
    } as Selectable<IChangeLog>;

    changeLogService.findById.mockResolvedValue(mockChangeLog);

    const response = await fastify.inject({
      method: 'GET',
      url: '/change-log/13',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(200);

    const result = response.body;
    const parsed = JSON.parse(result);
    expect(parsed.id).toEqual(13);
    expect(parsed.entity).toEqual('employee');
    expect(parsed.changeHistory.user).toEqual(1);
    expect(parsed.changeHistory.description).toEqual('Test');
    expect(changeLogService.findById).toHaveBeenCalledWith(13);
  });

  test('should handle get all changeLogs', async () => {
    const mockChangeLogs = [
      {
        id: 13,
        tenant: 1,
        project: 2,
        entity: "employee",
        createdByUser: 2,
        entitySchema: {},
        associatedApprovedEntity: null,
        createdOn: new Date("2025-08-28T07:21:33.998Z"),
        changeHistory: {
          user: 1,
          changeType: "create",
          description: "Test",
          timestamp: new Date("2025-08-28T07:21:33.998Z"),
          approvalHistory: []
        } as ChangeHistory
      }
    ] as Selectable<IChangeLog>[];

    changeLogService.findAll.mockResolvedValue(mockChangeLogs);

    const response = await fastify.inject({
      method: 'GET',
      url: '/change-log',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(200);

    const result = response.body;

    const parsed = JSON.parse(result);
    expect(Array.isArray(parsed)).toBe(true);
    expect(parsed).toHaveLength(1);
    const firstEntity = parsed[0];
    expect(firstEntity.entity).toEqual('employee');
    expect(firstEntity.changeHistory.user).toEqual(1);
    expect(firstEntity.changeHistory.description).toEqual('Test');
    expect(firstEntity.id).toEqual(13);
    expect(firstEntity.tenant).toEqual(1);

    expect(firstEntity.createdOn).toEqual("2025-08-28T07:21:33.998Z");
    expect(firstEntity.changeHistory.timestamp).toEqual("2025-08-28T07:21:33.998Z");
    expect(changeLogService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create ChangeLog', async () => {
    const newChangeLog: Insertable<IChangeLog> = {
      tenant: 1,
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
      createdByUser: 501,
      associatedApprovedEntity: 1
    };

    const requestPayload: 
      Omit<InsertableEntity<IChangeLog>, 'createdByUser' | 'changeHistory' | 'entitySchema'> & { changeHistory: ChangeHistoryPayload; entitySchema: any } = {
      project: newChangeLog.project,
      entity: newChangeLog.entity,
      associatedApprovedEntity: newChangeLog.associatedApprovedEntity,
      entitySchema: {},
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Approved By User",
        timestamp: (new Date()).toISOString(),
        approvalHistory: [
          {
            user: 1,
            timestamp: (new Date()).toISOString(),
            description: "Approved By User",
            status: "approved"
          }
        ]
      }
    };
    
    const createdChangeLog: Selectable<IChangeLog> = { 
      id: 2, 
      ...newChangeLog, 
      createdByUser: 501,
      associatedApprovedEntity: 1,
      entitySchema: {},
      changeHistory: {
        user: 501,
        changeType: "create",
        description: "sample",
        timestamp: new Date(),
        approvalHistory: []
      },
      createdOn: new Date(),
    };

    changeLogService.create.mockResolvedValue(createdChangeLog);

    const response = await fastify.inject({
      method: 'POST',
      url: '/change-log',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({ 
      ...createdChangeLog, 
      createdOn: createdChangeLog.createdOn.toISOString(),
      changeHistory: { 
        ...createdChangeLog.changeHistory, 
        timestamp: createdChangeLog.changeHistory.timestamp.toISOString() 
      }
    }); // to get date string format correct for created on and timestamp
    expect(changeLogService.create).toHaveBeenCalledWith(requestPayload);
  });


  test('should handle Change log not found.', async () => {
    changeLogService.findById.mockResolvedValue(undefined);

    const response = await fastify.inject({
      method: 'GET',
      url: '/change-log/999',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'Change log not found.' });
    expect(changeLogService.findById).toHaveBeenCalledWith(999);
  });

  test('should handle changeLog creation db error', async () => {
    const newChangeLog: Omit<InsertableEntity<IChangeLog>, 'createdByUser' | 'changeHistory' | 'entitySchema'> & {
      changeHistory: ChangeHistoryPayload;
      entitySchema: any
    } = {
      project: 201,
      entity: "employee",
      entitySchema: {},
      associatedApprovedEntity: null,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Approved By User",
        timestamp: (new Date()).toISOString(),
        approvalHistory: [
          {
            user: 1,
            timestamp: (new Date()).toISOString(),
            description: "Approved By User",
            status: "approved"
          }
        ]
      },
    };

    const requestPayload: Omit<InsertableEntity<IChangeLog>, 'createdByUser' | 'changeHistory' | 'entitySchema'> & {
      changeHistory: ChangeHistoryPayload;
      entitySchema: any
    } = { ...newChangeLog };

    changeLogService.create.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'POST',
      url: '/change-log',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(changeLogService.create).toHaveBeenCalledWith(requestPayload);
  });


});

describe('Default -> ChangeLog -> ChangeLogRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let changeLogService: DeepMockProxy<ChangeLogService>;

  beforeAll(async () => {
    fastify = Fastify({ logger: false }) as unknown as FastifyInstance;
    changeLogService = mockDeep<ChangeLogService>();

    // Mock the changeLogService methods
    fastify.decorateRequest('changeLogService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<ChangeLogService>('changeLogService', changeLogService);
    });

    fastify.setErrorHandler(async (error: Error, request, reply) => {
      let statusCode = 500;
      let errorMessage = 'Internal Server Error';

      if (error instanceof CustomError) {
        statusCode = error.statusCode;
        errorMessage = error.message;
      }

      reply.code(statusCode).send({ error: errorMessage });
    });

    fastify.addHook('preHandler', async (request, reply) => {
      throw new CustomError('Forbidden', 403);
    });
    await fastify.register(changeLogRoutes, { prefix: '/change-log' });
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should handle get changeLog by id if Forbidden', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/change-log/1',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle get all changeLogs', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/change-log',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle create changeLog', async () => {
    const newChangeLog = {
      tenant: 1,
      project: 201,
      entity: "employee",
      createdByUser: 501,
      entitySchema: {},
      changeHistory: {}
    } as Insertable<IChangeLog>;

    const requestPayload = { ...newChangeLog, entitySchema: {}, changeHistory: {} } as Insertable<IChangeLog>;

    const response = await fastify.inject({
      method: 'POST',
      url: '/change-log',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

 

});