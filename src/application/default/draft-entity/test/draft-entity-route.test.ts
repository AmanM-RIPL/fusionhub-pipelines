import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { DraftEntityService } from "../draft-entity.service";
import draftEntityRoutes from "../draft-entity.route";
import { Insertable, Selectable } from "kysely";
import { IDraftEntity } from "../draft-entity.model";
import { UpdateableEntity, ChangeHistory, ColumnValue, InsertableEntity, ChangeHistoryPayload } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> DraftEntity -> DraftEntityRoute', () => {
  let fastify: FastifyInstance;
  let draftEntityService: DeepMockProxy<DraftEntityService>;

  beforeAll(async () => {
    fastify = Fastify({ logger: false }) as unknown as FastifyInstance;
    draftEntityService = mockDeep<DraftEntityService>();

    // Mock the draftEntityService methods
    fastify.decorateRequest('draftEntityService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<DraftEntityService>('draftEntityService', draftEntityService);
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
    await draftEntityRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should register draftEntity routes', async () => {
    const routes = fastify.printRoutes();
    expect(routes).toContain(':draftEntityId');
    expect(routes).toContain('draftEntity');
  });

  test('should handle get draftEntity by id', async () => {
    const mockDraftEntity = {
      id: 13,
      tenant: 1,
      project: 2,
      entity: "employee",
      createdByUser: 2,
      nextApprovingUser: 2,
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
    } as Selectable<IDraftEntity>;

    draftEntityService.findById.mockResolvedValue(mockDraftEntity);

    const response = await fastify.inject({
      method: 'GET',
      url: '/draft-entity/13',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(200);

    const result = response.body;
    const parsed = JSON.parse(result);
    expect(parsed.id).toEqual(13);
    expect(parsed.entity).toEqual('employee');
    expect(parsed.changeHistory.user).toEqual(1);
    expect(parsed.changeHistory.description).toEqual('Test');
    expect(draftEntityService.findById).toHaveBeenCalledWith(13);
  });

  test('should handle get all draftEntitys', async () => {
    const mockDraftEntitys = [
      {
        id: 13,
        tenant: 1,
        project: 2,
        entity: "employee",
        createdByUser: 2,
        nextApprovingUser: 2,
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
    ] as Selectable<IDraftEntity>[];

    draftEntityService.findAll.mockResolvedValue(mockDraftEntitys);

    const response = await fastify.inject({
      method: 'GET',
      url: '/draft-entity',
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
    expect(draftEntityService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create DraftEntity', async () => {
    const newDraftEntity: Insertable<IDraftEntity> = {
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
      nextApprovingUser: 502,
      associatedApprovedEntity: 1
    };

    const requestPayload: 
      Omit<InsertableEntity<IDraftEntity>, 'createdByUser' | 'changeHistory' | 'entitySchema' | 'nextApprovingUser'> & { changeHistory: ChangeHistoryPayload; entitySchema: any } = {
      project: newDraftEntity.project,
      entity: newDraftEntity.entity,
      associatedApprovedEntity: newDraftEntity.associatedApprovedEntity,
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
    
    const createdDraftEntity: Selectable<IDraftEntity> = { 
      id: 2, 
      ...newDraftEntity, 
      createdByUser: 501,
      nextApprovingUser: 502,
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

    draftEntityService.create.mockResolvedValue(createdDraftEntity);

    const response = await fastify.inject({
      method: 'POST',
      url: '/draft-entity',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({ 
      ...createdDraftEntity, 
      createdOn: createdDraftEntity.createdOn.toISOString(),
      changeHistory: { 
        ...createdDraftEntity.changeHistory, 
        timestamp: createdDraftEntity.changeHistory.timestamp.toISOString() 
      }
    }); // to get date string format correct for created on and timestamp
    expect(draftEntityService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update draftEntity', async () => {
    const updatedDraftEntity = {
      firstName: "John",
      lastName: "Doe",
      email: "john.doe@example.com"
    };

    const updatedDraftEntityResult: Selectable<IDraftEntity> = {
      id: 13,
      tenant: 1,
      project: 2,
      entity: "employee",
      createdByUser: 2,
      nextApprovingUser: 2,
      entitySchema: updatedDraftEntity,
      associatedApprovedEntity: null,
      createdOn: new Date(),
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: []
      }
    };

    draftEntityService.update.mockResolvedValue(updatedDraftEntityResult);

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/draft-entity/13',
      headers: {
        authorization: 'Bearer test-token'
      },
      payload: updatedDraftEntity
    });

    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({
      ...updatedDraftEntityResult,
      createdOn: updatedDraftEntityResult.createdOn.toISOString(),
      changeHistory: {
        ...updatedDraftEntityResult.changeHistory,
        timestamp: updatedDraftEntityResult.changeHistory.timestamp.toISOString()
      }
    });
    expect(draftEntityService.update).toHaveBeenCalledWith(13, updatedDraftEntity);
  });

  test('should handle draft entity not found', async () => {
    draftEntityService.findById.mockResolvedValue(undefined);

    const response = await fastify.inject({
      method: 'GET',
      url: '/draft-entity/999',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'Draft entity not found' });
    expect(draftEntityService.findById).toHaveBeenCalledWith(999);
  });

  test('should handle draftEntity creation db error', async () => {
    const newDraftEntity: Omit<InsertableEntity<IDraftEntity>, 'createdByUser' | 'changeHistory' | 'entitySchema' | 'nextApprovingUser'> & {
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

    const requestPayload: Omit<InsertableEntity<IDraftEntity>, 'createdByUser' | 'changeHistory' | 'entitySchema' | 'nextApprovingUser'> & {
      changeHistory: ChangeHistoryPayload;
      entitySchema: any
    } = { ...newDraftEntity };

    draftEntityService.create.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'POST',
      url: '/draft-entity',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(draftEntityService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle draftEntity update db error', async () => {
    const updatedDraftEntity = {
      tenant: 1,
      project: 101,
      entity: "employee",
      createdByUser: 501,
      nextApprovingUser: 502,
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({})
    } as UpdateableEntity<IDraftEntity>;

    draftEntityService.update.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/draft-entity/1',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: updatedDraftEntity
    });

    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
  });

  test('should successfully delete draft entity when validation passes', async () => {
    const mockDraftEntity = {
      id: 13,
      tenant: 1,
      createdOn: new Date(),
      project: 2,
      entity: "employee",
      createdByUser: 2,
      nextApprovingUser: null, // No approving user assigned
      entitySchema: {},
      associatedApprovedEntity: null,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: []
      }
    } as Selectable<IDraftEntity>;

    draftEntityService.findById.mockResolvedValue(mockDraftEntity);
    draftEntityService.delete.mockResolvedValue(undefined);

    const response = await fastify.inject({
      method: 'DELETE',
      url: '/draft-entity/13',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(204);
    expect(draftEntityService.delete).toHaveBeenCalledWith(13);
  });

  test('should return 500 when draft entity has next approving user assigned', async () => {
    const mockDraftEntity = {
      id: 13,
      tenant: 1,
      createdOn: new Date(),
      project: 2,
      entity: "employee",
      createdByUser: 2,
      nextApprovingUser: 3, // Has approving user assigned - block deletion
      entitySchema: {},
      associatedApprovedEntity: null,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: []
      }
    } as Selectable<IDraftEntity>;

    draftEntityService.findById.mockResolvedValue(mockDraftEntity);
    draftEntityService.delete.mockRejectedValue(
      new Error("Cannot delete draft entity 13 because it has a next approving user assigned.")
    );

    const response = await fastify.inject({
      method: 'DELETE',
      url: '/draft-entity/13',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(500);
    const result = JSON.parse(response.body);
    expect(result.error).toBe("Internal Server Error");
  });

  test('should return 500 when user is not the creator of the draft', async () => {
    const mockDraftEntity = {
      id: 13,
      tenant: 1,
      createdOn: new Date(),
      project: 2,
      entity: "employee",
      createdByUser: 5, // Different user created it - only creator can delete
      nextApprovingUser: null,
      entitySchema: {},
      associatedApprovedEntity: null,
      changeHistory: {
        user: 1,
        changeType: "create",
        description: "Test",
        timestamp: new Date(),
        approvalHistory: []
      }
    } as Selectable<IDraftEntity>;

    draftEntityService.findById.mockResolvedValue(mockDraftEntity);
    draftEntityService.delete.mockRejectedValue(new Error("Only the user who created this draft (User ID: 5) can delete it."));

    const response = await fastify.inject({
      method: 'DELETE',
      url: '/draft-entity/13',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(500);
    const result = JSON.parse(response.body);
    expect(result.error).toBe("Internal Server Error");
  });

});

describe('Default -> DraftEntity -> DraftEntityRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let draftEntityService: DeepMockProxy<DraftEntityService>;

  beforeAll(async () => {
    fastify = Fastify({ logger: false }) as unknown as FastifyInstance;
    draftEntityService = mockDeep<DraftEntityService>();

    // Mock the draftEntityService methods
    fastify.decorateRequest('draftEntityService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<DraftEntityService>('draftEntityService', draftEntityService);
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
    await fastify.register(draftEntityRoutes, { prefix: '/draft-entity' });
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should handle get draftEntity by id if Forbidden', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/draft-entity/1',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle get all draftEntitys', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/draft-entity',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle create draftEntity', async () => {
    const newDraftEntity = {
      tenant: 1,
      project: 201,
      entity: "employee",
      createdByUser: 501,
      nextApprovingUser: 502,
      entitySchema: {},
      changeHistory: {}
    } as Insertable<IDraftEntity>;

    const requestPayload = { ...newDraftEntity, entitySchema: {}, changeHistory: {} } as Insertable<IDraftEntity>;

    const response = await fastify.inject({
      method: 'POST',
      url: '/draft-entity',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle update draftEntity', async () => {
    const updatedDraftEntity = {
      tenant: 1,
      project: 101,
      entity: "employee",
      createdByUser: 501,
      nextApprovingUser: 502,
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({})
    } as UpdateableEntity<IDraftEntity>;

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/draft-entity/1',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: updatedDraftEntity
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

});