import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ProjectService } from "../project.service";
import projectRoutes from "../project.route";
import { Insertable, Selectable } from "kysely";
import { IProject } from "../project.model";
import { UpdateableEntity } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> Project -> ProjectRoute', () => {
  let fastify: FastifyInstance;
  let projectService: DeepMockProxy<ProjectService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    projectService = mockDeep<ProjectService>();

    // Mock the projectService methods
    fastify.decorateRequest('projectService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<ProjectService>('projectService', projectService);
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

    await projectRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should register project routes', async () => {
    const routes = fastify.printRoutes();
    expect(routes).toContain(':projectId');
    expect(routes).toContain('project');
  });

  test('should handle get project by id', async () => {
    const mockProject = {
      id: 1,
      name: 'HRMS System',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890',
      isBlocked: false
    } as Selectable<IProject>;

    projectService.findById.mockResolvedValue(mockProject);

    const response = await fastify.inject({
      method: 'GET',
      url: '/project/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({ ...mockProject, endDate: mockProject.endDate?.toISOString().split('T')[0], startDate: mockProject.startDate?.toISOString().split('T')[0], createdOn: mockProject.createdOn.toISOString() }); // to get date string format correct
    expect(projectService.findById).toHaveBeenCalledWith(1);
  });

  test('should handle get all projects', async () => {
    const mockProjects = [
      {
        id: 1,
        name: 'HRMS System',
        createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
        description: 'A human resource management system for internal use.',
        projectCode: 'HRMS001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'hrms@company.com',
        contactMobile: '+911234567890',
        isBlocked: false
      }
    ] as Selectable<IProject>[];

    projectService.findAll.mockResolvedValue(mockProjects);

    const response = await fastify.inject({
      method: 'GET',
      url: '/project',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual(mockProjects.map(project => ({ ...project, endDate: project.endDate?.toISOString().split('T')[0], startDate: project.startDate?.toISOString().split('T')[0], createdOn: project.createdOn.toISOString() })));
    expect(projectService.findAll).toHaveBeenCalledWith(10, 0);
  });


  test('should handle create project', async () => {
    const newProject = {
      tenant: 1,
      name: 'HRMS System',
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890'
    }as Insertable<IProject>;
        const requestPayload = { 
          ...newProject
        } as Insertable<IProject>

    const createdProject = { id: 1, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;

    projectService.create.mockResolvedValue(createdProject);

    const response = await fastify.inject({
      method: 'POST',
      url: '/project',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

     expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({...createdProject, createdOn: createdProject.createdOn.toISOString()}); // to get date string format correct
    expect(projectService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update project', async () => {
    const updatedProject = {
      tenant: 1,
      name: 'HRMS System',
      createdOn: new Date(),
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890'
    } as UpdateableEntity<IProject>;
    const updatedProjectResult = { id: 1, ...updatedProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;
        
        projectService.update.mockResolvedValue(updatedProjectResult);
        
        const response = await fastify.inject({
          method: 'PATCH',
          url: '/project/1',
          headers: { authorization: 'Bearer test-token' },
          payload: updatedProject
        });
    
        // Check the response
        expect(response.statusCode).toBe(200);
        expect(JSON.parse(response.body)).toEqual({...updatedProjectResult, createdOn: updatedProjectResult.createdOn.toISOString()}); // to get date string format correct
        expect(projectService.update).toHaveBeenCalledWith(1, updatedProject);
  });

  test('should handle project not found', async () => {
    projectService.findById.mockResolvedValue(undefined);


    const response = await fastify.inject({
      method: 'GET',
      url: '/project/999',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'Project not found' });
  });

  test('should handle project creation db error', async () => {
    const newProject = {
      tenant: 1,
      name: 'HRMS System',
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890'
    } as Insertable<IProject>;
    const requestPayload = {
      ...newProject
    } as Insertable<IProject>;
    const createdProject = { id: 2, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;

    projectService.create.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'POST',
      url: '/project',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(projectService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle project update db error', async () => {
    const updatedProject = {
      tenant: 1,
      name: 'HRMS System',
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890'
    } as UpdateableEntity<IProject>;
    projectService.update.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/project/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(projectService.update).toHaveBeenCalledWith(1, updatedProject);
  });
});

describe('Default -> Project -> ProjectRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let projectService: DeepMockProxy<ProjectService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    projectService = mockDeep<ProjectService>();

    // Mock the projectService methods
    fastify.decorateRequest('projectService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<ProjectService>('projectService', projectService);
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

    await projectRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should handle get project by id', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/project/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle get all projects', async () => {

    const response = await fastify.inject({
      method: 'GET',
      url: '/project',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });

  test('should handle create project', async () => {
    const newProject = {
      name: 'HRMS System',
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890'
    } as Insertable<IProject>;
    const requestPayload = {
      ...newProject
    } as Insertable<IProject>;
    const createdProject = { id: 2, ...newProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;

    projectService.create.mockResolvedValue(createdProject);

    const response = await fastify.inject({
      method: 'POST',
      url: '/project',
      headers: { authorization: 'Bearer test-token' },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
  });

  test('should handle update project', async () => {
    const updatedProject = {
      tenant: 1,
      name: 'HRMS System',
      description: 'A human resource management system for internal use.',
      projectCode: 'HRMS001',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      contactEmail: 'hrms@company.com',
      contactMobile: '+911234567890',
      isBlocked: false
    } as UpdateableEntity<IProject>;
    const updatedProjectResult = { id: 1, ...updatedProject, createdOn: new Date(), isBlocked: false } as Selectable<IProject>;

    projectService.update.mockResolvedValue(updatedProjectResult);

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/project/1',
      headers: { authorization: 'Bearer test-token' },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });
});