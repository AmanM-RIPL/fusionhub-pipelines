import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ProjectService } from "../project.service";
import tenantRoutes from "../project.route";
import { Insertable, Selectable } from "kysely";
import { IProject } from "../project.model";
import { UpdateableEntity } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> Project -> ProjectRoute', () => {
  let fastify: FastifyInstance;
  let tenantService: DeepMockProxy<ProjectService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    tenantService = mockDeep<ProjectService>();

    // Mock the tenantService methods
    fastify.decorateRequest('tenantService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<ProjectService>('tenantService', tenantService);
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
    const mockProject = { 
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false
    } as Selectable<IProject>;

    tenantService.findById.mockResolvedValue(mockProject);

    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({...mockProject, createdOn: mockProject.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.findById).toHaveBeenCalledWith(1);
  });

  test('should handle get all tenants', async () => {
    const mockProjects = [{ 
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false
    } as Selectable<IProject>];
    
    tenantService.findAll.mockResolvedValue(mockProjects);

    const response = await fastify.inject({
      method: 'GET',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual(mockProjects.map(tenant => ({ ...tenant, createdOn: tenant.createdOn.toISOString() }))); // to get date string format correct
    expect(tenantService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create tenant', async () => {
    const newProject = { 
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    } as Insertable<IProject>;
    const requestPayload = { 
      ...newProject, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<IProject> & { adminUsername: string, adminPassword: string };
    const createdProject = { id: 2, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
    
    tenantService.create.mockResolvedValue(createdProject);
    
    const response = await fastify.inject({
      method: 'POST',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({...createdProject, createdOn: createdProject.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update tenant', async () => {
    const updatedProject = { name: 'Updated Project', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<IProject>;
    const updatedProjectResult = { id: 1, ...updatedProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
    
    tenantService.update.mockResolvedValue(updatedProjectResult);
    
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({...updatedProjectResult, createdOn: updatedProjectResult.createdOn.toISOString()}); // to get date string format correct
    expect(tenantService.update).toHaveBeenCalledWith(1, updatedProject);
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
    expect(JSON.parse(response.body)).toEqual({ error: 'Project not found' });
  });

  test('should handle tenant creation db error', async () => {
    const newProject = { 
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    } as Insertable<IProject>;
    const requestPayload = { 
      ...newProject, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<IProject> & { adminUsername: string, adminPassword: string };
    const createdProject = { id: 2, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
    
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
    const updatedProject = { name: 'Updated Project', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<IProject>;
    tenantService.update.mockRejectedValue(new Error('Database error'));
    
    const response = await fastify.inject({ 
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(tenantService.update).toHaveBeenCalledWith(1, updatedProject);
  });
});

describe('Default -> Project -> ProjectRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let tenantService: DeepMockProxy<ProjectService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    tenantService = mockDeep<ProjectService>();

    // Mock the tenantService methods
    fastify.decorateRequest('tenantService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 2, tenant: 1 } as RequestUser;
      request.setDecorator<ProjectService>('tenantService', tenantService);
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
    const newProject = { 
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    } as Insertable<IProject>;
    const requestPayload = { 
      ...newProject, 
      adminUsername: 'newuser', 
      adminPassword: 'sample' 
    } as Insertable<IProject> & { adminUsername: string, adminPassword: string };
    const createdProject = { id: 2, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
    
    tenantService.create.mockResolvedValue(createdProject);
    
    const response = await fastify.inject({
      method: 'POST',
      url: '/tenant',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
  });

  test('should handle update tenant', async () => {
    const updatedProject = { name: 'Updated Project', defaultEmail: 'sample@gmail.com', defaultMobile: '1234567890' } as UpdateableEntity<IProject>;
    const updatedProjectResult = { id: 1, ...updatedProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
    
    tenantService.update.mockResolvedValue(updatedProjectResult);
    
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/tenant/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });
});