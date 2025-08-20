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

  const expected = {
    ...mockDraftEntity,
    createdOn: mockDraftEntity.createdOn.toISOString(),
    changeHistory: {},
  };
  delete (expected as any).isBlocked;

  expect(JSON.parse(response.body)).toEqual(expected);
  expect(draftEntityService.findById).toHaveBeenCalledWith(13);
});


  test('should handle get all draftEntitys', async () => {
    const mockDraftEntitys = [
      {
        id: 1,
        tenant: 1,
        project: 101,
        entity: "employee",
        createdByUser: 1,
        nextApprovingUser: 2,
        entitySchema: {},
        associatedApprovedEntity: null,
        createdOn: new Date(),
        changeHistory: {} as ChangeHistory
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
    expect(JSON.parse(response.body)).toEqual(mockDraftEntitys.map(draftEntity => ({ 
      ...draftEntity, 
      createdOn: draftEntity.createdOn.toISOString() 
    })));
    expect(draftEntityService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create draftEntity', async () => {
    const newDraftEntity = {
      tenant: 1,
      project: 201,
      entity: "employee",
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({}),
      createdByUser: 501,
      nextApprovingUser: 502,
      associatedApprovedEntity: 1,
    } as Insertable<IDraftEntity>;
    
    const requestPayload = {...newDraftEntity, entitySchema: {}, changeHistory: {} } as Insertable<IDraftEntity>;

    const createdDraftEntity = {
      ...newDraftEntity,
      id: 1,
      createdOn: new Date(),
      entitySchema: JSON.parse(newDraftEntity.entitySchema),
      changeHistory: JSON.parse(newDraftEntity.changeHistory) as ChangeHistory 
    } as Selectable<IDraftEntity>;

    draftEntityService.create.mockResolvedValue(createdDraftEntity);

    const response = await fastify.inject({
      method: 'POST',
      url: '/draft-entity',
      headers: { 
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({
      ...createdDraftEntity, 
      createdOn: createdDraftEntity.createdOn.toISOString()
    });
    expect(draftEntityService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update draftEntity', async () => {     
  const updatedDraftEntity = {       
    tenant: 1,       
    project: 201,       
    entity: "employee",       
    entitySchema: {},
    changeHistory: {},
    createdByUser: 501,       
    nextApprovingUser: 502,       
    associatedApprovedEntity: 1,     
  };          

  const updatedDraftEntityResult = {        
    ...updatedDraftEntity,       
    id: 13,       
    createdOn: new Date(),      
    entitySchema: updatedDraftEntity.entitySchema,
    changeHistory: updatedDraftEntity.changeHistory
  } as Selectable<IDraftEntity>;              

  draftEntityService.update.mockResolvedValue(updatedDraftEntityResult);              

  const response = await fastify.inject({       
    method: 'PATCH',       
    url: '/draft-entity/13',       
    headers: {          
      authorization: 'Bearer test-token',         
      'content-type': 'application/json'       
    },       
    payload: updatedDraftEntity     
  });      

  expect(response.statusCode).toBe(200);     
  expect(JSON.parse(response.body)).toEqual({       
    ...updatedDraftEntityResult,        
    createdOn: updatedDraftEntityResult.createdOn.toISOString()     
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
    const newDraftEntity = {
      tenant: 1,
      project: 201,
      entity: "employee",
      createdByUser: 501,
      nextApprovingUser: 502,
      entitySchema: JSON.stringify({}),
      changeHistory: JSON.stringify({})
    } as Insertable<IDraftEntity>;
    
    const requestPayload = {...newDraftEntity, entitySchema: {}, changeHistory: {} } as Insertable<IDraftEntity>;

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
    
    const requestPayload = {...newDraftEntity, entitySchema: {}, changeHistory: {} }  as Insertable<IDraftEntity>;

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