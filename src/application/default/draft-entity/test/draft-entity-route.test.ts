import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { DraftEntityService } from "../draft-entity.service";
import draftEntityRoutes from "../draft-entity.route";
import { Insertable, Selectable } from "kysely";
import { IDraftEntity } from "../draft-entity.model";
import { UpdateableEntity, ChangeHistory, ColumnValue  } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> DraftEntity -> DraftEntityRoute', () => {
  let fastify: FastifyInstance;
  let draftEntityService: DeepMockProxy<DraftEntityService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
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
        id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502,
        entitySchema: {},
        associatedApprovedEntity: null,
        parentDraftEntity: null
        } as Selectable<IDraftEntity>;


  draftEntityService.findById.mockResolvedValue(mockDraftEntity);

  const response = await fastify.inject({
    method: 'GET',
    url: '/draftEntity/1',
    headers: { authorization: 'Bearer test-token' }
  });

  expect(response.statusCode).toBe(200);
  expect(JSON.parse(response.body)).toEqual({
    ...mockDraftEntity,
    createdOn: mockDraftEntity.createdOn.toISOString(),
  });
  expect(draftEntityService.findById).toHaveBeenCalledWith(1);
});



  test('should handle get all draftEntitys', async () => {
    const mockDraftEntitys = [
      {
       id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502,
        entitySchema: {},
        associatedApprovedEntity: null,
        parentDraftEntity: null
      }
    ] as Selectable<IDraftEntity>[];

    draftEntityService.findAll.mockResolvedValue(mockDraftEntitys);

    const response = await fastify.inject({
      method: 'GET',
      url: '/draftEntity',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual(mockDraftEntitys.map(draftEntity => ({ ...draftEntity, createdOn: draftEntity.createdOn.toISOString() })));
    expect(draftEntityService.findAll).toHaveBeenCalledWith(10, 0);
  });


  test('should handle create draftEntity', async () => {
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
        createdByUser: 501,
        nextApprovingUser: 502,
        associatedApprovedEntity: 1,
        parentDraftEntity: null
    }as Insertable<IDraftEntity>;
        const requestPayload = { ...newDraftEntity } as Insertable<IDraftEntity>;

   const createdDraftEntity = {...newDraftEntity,id: 1,createdOn: new Date(),isBlocked: false,entitySchema: JSON.parse(newDraftEntity.entitySchema),changeHistory: JSON.parse(newDraftEntity.changeHistory) as ChangeHistory,data: JSON.parse(newDraftEntity.data) as ColumnValue } as Selectable<IDraftEntity>;

    draftEntityService.create.mockResolvedValue(createdDraftEntity);

    const response = await fastify.inject({
      method: 'POST',
      url: '/draftEntity',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

     expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({...createdDraftEntity, createdOn: createdDraftEntity.createdOn.toISOString()}); // to get date string format correct
    expect(draftEntityService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update draftEntity', async () => {

const updatedDraftEntity = {
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
        createdByUser: 501,
        nextApprovingUser: 502,
        associatedApprovedEntity: 1,
        parentDraftEntity: null
   } as UpdateableEntity<IDraftEntity>;
    const updatedDraftEntityResult = { ...updatedDraftEntity ,id: 1,createdOn: new Date(),isBlocked: false,entitySchema: JSON.parse(updatedDraftEntity.entitySchema ?? "{}"),changeHistory: JSON.parse(updatedDraftEntity.changeHistory ?? "{}") as ChangeHistory,data: JSON.parse(updatedDraftEntity.data ?? "{}") as ColumnValue} as Selectable<IDraftEntity>;
        
        draftEntityService.update.mockResolvedValue(updatedDraftEntityResult);
        
        const response = await fastify.inject({
          method: 'PATCH',
          url: '/draftEntity/1',
          headers: { authorization: 'Bearer test-token' },
          payload: updatedDraftEntity
        });
    
        // Check the response
        expect(response.statusCode).toBe(200);
        expect(JSON.parse(response.body)).toEqual({...updatedDraftEntityResult, createdOn: updatedDraftEntityResult.createdOn.toISOString()}); // to get date string format correct
        expect(draftEntityService.update).toHaveBeenCalledWith(1, updatedDraftEntity);
  });

  test('should handle draftEntity not found', async () => {
    draftEntityService.findById.mockResolvedValue(undefined);


    const response = await fastify.inject({
      method: 'GET',
      url: '/draftEntity/999',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'DraftEntity not found' });
  });

  test('should handle draftEntity creation db error', async () => {
    const newDraftEntity = {
        id: 1,
        tenant: 1,
        project: 201,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502
    } as Insertable<IDraftEntity>;
    const requestPayload = {
      ...newDraftEntity
    } as Insertable<IDraftEntity>;
    const createdDraftEntity = {id:2, ...newDraftEntity ,createdOn: new Date(),isBlocked: false,entitySchema: JSON.parse(newDraftEntity.entitySchema ?? "{}"),changeHistory: JSON.parse(newDraftEntity.changeHistory ?? "{}") as ChangeHistory,data: JSON.parse(newDraftEntity.data ?? "{}") as ColumnValue } as Selectable<IDraftEntity>;

    draftEntityService.create.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'POST',
      url: '/draftEntity',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(draftEntityService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle draftEntity update db error', async () => {
    const updatedDraftEntity = {
        id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502
        
    } as UpdateableEntity<IDraftEntity>;
    draftEntityService.update.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/draftEntity/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedDraftEntity
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(draftEntityService.update).toHaveBeenCalledWith(1, updatedDraftEntity);
  });
});

describe('Default -> DraftEntity -> DraftEntityRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let draftEntityService: DeepMockProxy<DraftEntityService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
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

  test('should handle get draftEntity by id', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/draftEntity/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle get all draftEntitys', async () => {

    const response = await fastify.inject({
      method: 'GET',
      url: '/draftEntity',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle create draftEntity', async () => {
    const newDraftEntity = {
        id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502
    } as Insertable<IDraftEntity>;
    const requestPayload = {
      ...newDraftEntity
    } as Insertable<IDraftEntity>;
    const createdDraftEntity = { id:2, ...newDraftEntity ,createdOn: new Date(),isBlocked: false,entitySchema: JSON.parse(newDraftEntity.entitySchema ?? "{}"),changeHistory: JSON.parse(newDraftEntity.changeHistory ?? "{}") as ChangeHistory,data: JSON.parse(newDraftEntity.data ?? "{}") as ColumnValue  } as Selectable<IDraftEntity>;

    draftEntityService.create.mockResolvedValue(createdDraftEntity);

    const response = await fastify.inject({
      method: 'POST',
      url: '/draftEntity',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
  });

  test('should handle update draftEntity', async () => {
    const updatedDraftEntity = {
       id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 501,
        nextApprovingUser: 502
    } as UpdateableEntity<IDraftEntity>;
    const updatedDraftEntityResult = { id: 2, ...updatedDraftEntity, ...updatedDraftEntity ,createdOn: new Date(),isBlocked: false,entitySchema: JSON.parse(updatedDraftEntity.entitySchema ?? "{}"),changeHistory: JSON.parse(updatedDraftEntity.changeHistory ?? "{}") as ChangeHistory,data: JSON.parse(updatedDraftEntity.data ?? "{}") as ColumnValue } as Selectable<IDraftEntity>;

    draftEntityService.update.mockResolvedValue(updatedDraftEntityResult);

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/draftEntity/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedDraftEntity
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });
});