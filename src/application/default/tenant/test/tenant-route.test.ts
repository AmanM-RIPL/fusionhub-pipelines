import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { TenantService } from "../tenant.service";
import tenantRoutes from "../tenant.route";
import { Insertable, Selectable } from "kysely";
import { ITenant } from "../tenant.model";
import { UpdateableEntity } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> Tenant -> TenantRoute', () => {
  let fastify: FastifyInstance;
  let tenantService: DeepMockProxy<TenantService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    tenantService = mockDeep<TenantService>();

    // Mock the tenantService methods
    fastify.decorateRequest('tenantService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<TenantService>('tenantService', tenantService);
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

    await tenantRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should register tenant routes', async () => {
    const routes = fastify.printRoutes();
    expect(routes).toContain(':tenantId');
    expect(routes).toContain('tenant');
  });

  test('should handle get tenant by id', async () => {
    const mockTenant = { 
      id: 1, 
      name: 'Test Tenant', 
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')), 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '1234567890', 
      isBlocked: false 
    } as Selectable<ITenant>;

    tenantService.findById.mockResolvedValue(mockTenant);

    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({...mockTenant, createdOn: mockTenant.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.findById).toHaveBeenCalledWith(1);
  });

  test('should handle get all tenants', async () => {
    const mockTenants = [{ 
      id: 1, 
      name: 'Test Tenant', 
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')), 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '1234567890', 
      isBlocked: false 
    } as Selectable<ITenant>];
    
    tenantService.findAll.mockResolvedValue(mockTenants);

    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual(mockTenants.map(tenant => ({ ...tenant, createdOn: tenant.createdOn.toISOString() }))); // to get date string format correct
    expect(tenantService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create tenant', async () => {
    const newTenant = { 
      name: 'New Tenant', 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '1234567890'
    } as Insertable<ITenant>;
    const requestPayload = { 
      ...newTenant, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<ITenant> & { adminUsername: string, adminPassword: string };
    const createdTenant = { id: 2, ...newTenant, createdOn: new Date(), isBlocked: false } as Selectable<ITenant>;
    
    tenantService.create.mockResolvedValue(createdTenant);
    
    const response = await fastify.inject({
      method: 'POST',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({...createdTenant, createdOn: createdTenant.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update tenant', async () => {
    const updatedTenant = { name: 'Updated Tenant', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<ITenant>;
    const updatedTenantResult = { id: 1, ...updatedTenant, createdOn: new Date(), isBlocked: false } as Selectable<ITenant>;
    
    tenantService.update.mockResolvedValue(updatedTenantResult);
    
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedTenant
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({...updatedTenantResult, createdOn: updatedTenantResult.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.update).toHaveBeenCalledWith(1, updatedTenant);
  });

  test('should handle tenant not found', async () => {
    tenantService.findById.mockResolvedValue(undefined);

    
    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant/999',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'Tenant not found' });
  });

  test('should handle tenant creation db error', async () => {
    const newTenant = { 
      name: 'New Tenant', 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '1234567890'
    } as Insertable<ITenant>;
    const requestPayload = { 
      ...newTenant, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<ITenant> & { adminUsername: string, adminPassword: string };
    const createdTenant = { id: 2, ...newTenant, createdOn: new Date(), isBlocked: false } as Selectable<ITenant>;
    
    tenantService.create.mockRejectedValue(new Error('Database error'));
    
    const response = await fastify.inject({
      method: 'POST',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(tenantService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle tenant update db error', async () => {
    const updatedTenant = { name: 'Updated Tenant', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<ITenant>;
    tenantService.update.mockRejectedValue(new Error('Database error'));
    
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedTenant
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(tenantService.update).toHaveBeenCalledWith(1, updatedTenant);
  });
});

describe('Default -> Tenant -> TenantRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let tenantService: DeepMockProxy<TenantService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    tenantService = mockDeep<TenantService>();

    // Mock the tenantService methods
    fastify.decorateRequest('tenantService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 2, tenant: 1 } as RequestUser;
      request.setDecorator<TenantService>('tenantService', tenantService);
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

    await tenantRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should handle get tenant by id', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle get all tenants', async () => {

    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle create tenant', async () => {
    const newTenant = { 
      name: 'New Tenant', 
      defaultEmail: 'sample@gmail.com', 
      defaultMobile: '1234567890'
    } as Insertable<ITenant>;
    const requestPayload = { 
      ...newTenant, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<ITenant> & { adminUsername: string, adminPassword: string };
    const createdTenant = { id: 2, ...newTenant, createdOn: new Date(), isBlocked: false } as Selectable<ITenant>;
    
    tenantService.create.mockResolvedValue(createdTenant);
    
    const response = await fastify.inject({
      method: 'POST',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
  });

  test('should handle update tenant', async () => {
    const updatedTenant = { name: 'Updated Tenant', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<ITenant>;
    const updatedTenantResult = { id: 1, ...updatedTenant, createdOn: new Date(), isBlocked: false } as Selectable<ITenant>;
    
    tenantService.update.mockResolvedValue(updatedTenantResult);
    
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedTenant
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });
});