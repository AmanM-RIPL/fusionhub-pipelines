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
    fastify = Fastify() as FastifyInstance;
    projectService = mockDeep<ProjectService>();

    fastify.decorateRequest('projectService', null);

    fastify.addHook('onRequest', async (request) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      (request as any).projectService = projectService;
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

    await projectRoutes(fastify);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  beforeEach(() => {
    // Reset all mocks before each test
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
      projectName: 'HRMS System',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      isBlocked: false
    } as Selectable<IProject>;
    projectService.findById.mockResolvedValue(mockProject);

    const response = await fastify.inject({
      method: 'GET',
      url: '/project/1',
      headers: { authorization: 'Bearer test-token' }
    });
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({ 
      ...mockProject, 
      endDate: mockProject.endDate?.toISOString().split('T')[0], 
      startDate: mockProject.startDate?.toISOString().split('T')[0], 
      createdOn: mockProject.createdOn.toISOString() 
    });
    expect(projectService.findById).toHaveBeenCalledWith(1);
  });

  test('should handle get all projects', async () => {
    const mockProjects = [
      {
      id: 1,
      projectName: 'HRMS System',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      lastChangeLogId: 1,
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
    expect(JSON.parse(response.body)).toEqual(mockProjects.map(project => ({ 
      ...project, 
      endDate: project.endDate?.toISOString().split('T')[0], 
      startDate: project.startDate?.toISOString().split('T')[0], 
      createdOn: project.createdOn.toISOString() 
    })));
    expect(projectService.findAll).toHaveBeenCalledWith(10, 0);
  });

 test('should handle create project', async () => { 
  const newProject = {
      tenant: 1,
      projectName: 'HRMS System',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      lastChangeLogId: 1,
      isBlocked: false
  } as Insertable<IProject>;

  const requestPayload = {
    ...newProject,
    startDate: "2025-01-01",
    endDate: "2025-12-31",
    createdOn: new Date('2025-01-01').toISOString(),
    isBlocked: false
  };

  const createdProject = {
    id: 1,
    ...newProject,
    createdOn: new Date('2025-01-01'),
    isBlocked: false
  } as Selectable<IProject>;

  projectService.create.mockResolvedValue(createdProject);
  
  const response = await fastify.inject({
    method: 'POST',
    url: '/project',
    headers: {
      authorization: 'Bearer test-token'
    },
    payload: requestPayload
  });

  expect(response.statusCode).toBe(201);
  expect(JSON.parse(response.body)).toEqual({
    ...createdProject,
    startDate: createdProject.startDate ? createdProject.startDate.toISOString().split('T')[0] : null,
    endDate: createdProject.endDate ? createdProject.endDate.toISOString().split('T')[0] : null,
    createdOn: createdProject.createdOn.toISOString()
  });

  expect(projectService.create).toHaveBeenCalledWith(expect.objectContaining({
    tenant: 1,
    projectName: 'HRMS System',
    description: 'A human resource management system for internal use.',
    startDate: "2025-01-01",
    endDate: "2025-12-31",
    status: 'active',
    customerName: 'company',
    customerEmail: 'hrms@company.com',
    customerMobile: '+911234567890',
    lastChangeLogId: 1,
    createdOn: "2025-01-01T00:00:00.000Z",
    isBlocked: false 
  }));
});

  test('should handle update project', async () => {
    const updatedProject = {
      tenant: 1,
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      lastChangeLogId: 1,
    } as UpdateableEntity<IProject>;
    
    const requestPayload = {
      ...updatedProject,
      startDate: updatedProject.startDate?.toISOString().split('T')[0],
      endDate: updatedProject.endDate?.toISOString().split('T')[0],
    };
    
    const updatedProjectResult = { 
      id: 1, 
      ...updatedProject, 
      createdOn: new Date(), 
      isBlocked: false 
    } as Selectable<IProject>;
        
    projectService.update.mockResolvedValue(updatedProjectResult);
        
    const response = await fastify.inject({
      method: 'PATCH',
      url: '/project/1',
      headers: { 
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({
      ...updatedProjectResult, 
      startDate: updatedProjectResult.startDate?.toISOString().split('T')[0],
      endDate: updatedProjectResult.endDate?.toISOString().split('T')[0],
      createdOn: updatedProjectResult.createdOn.toISOString()
    });
    // The service parsed dates (Date objects)
    expect(projectService.update).toHaveBeenCalledWith(1, expect.objectContaining({
      ...updatedProject,
      startDate: updatedProject.startDate?.toISOString().split('T')[0],
      endDate: updatedProject.endDate?.toISOString().split('T')[0],
    }));
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
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      isBlocked:false,
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      createdOn: new Date('2025-01-01'),
    } as Insertable<IProject>;
    
    const requestPayload = {
    ...newProject,
    startDate: "2025-01-01",
    endDate: "2025-12-31",
    createdOn: new Date('2025-01-01').toISOString(),
    isBlocked: false
  };

    projectService.create.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'POST',
      url: '/project',
      headers: { 
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });
    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(projectService.create).toHaveBeenCalledWith(expect.objectContaining({
    tenant: 1,
    projectName: 'HRMS System',
    description: 'A human resource management system for internal use.',
    startDate: "2025-01-01",
    endDate: "2025-12-31",
    status: 'active',
    customerName: 'company',
    customerEmail: 'hrms@company.com',
    customerMobile: '+911234567890',
    createdOn: "2025-01-01T00:00:00.000Z",
    isBlocked: false 
    }));
  });

  test('should handle project update db error', async () => {
    const updatedProject = {
      tenant: 1,
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.',
      startDate: new Date('2025-01-01'),
      endDate: new Date('2025-12-31'),
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      lastChangeLogId: 1,
    } as UpdateableEntity<IProject>;
    
    // Convert dates to strings for the request payload
    const requestPayload = {
      ...updatedProject,
      startDate: '2025-01-01',
      endDate: '2025-12-31'
    };
    
    projectService.update.mockRejectedValue(new Error('Database error'));

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/project/1',
      headers: { 
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: requestPayload
    });

    // Check the response
    expect(response.statusCode).toBe(500);
    expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
    expect(projectService.update).toHaveBeenCalledWith(1, expect.objectContaining({
      ...updatedProject,
      startDate: '2025-01-01',
      endDate: '2025-12-31'
    }));
  });
});

describe('Default -> Project -> ProjectRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let projectService: DeepMockProxy<ProjectService>;

  beforeAll(async () => {
    fastify = Fastify() as FastifyInstance;
    projectService = mockDeep<ProjectService>();

    // Mock the projectService methods
    fastify.decorateRequest('projectService', null);
    
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      (request as any).projectService = projectService;
      throw new CustomError('Forbidden', 403);
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

  beforeEach(() => {
    // Reset all mocks before each test
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

  test('should handle update project', async () => {
    const updatedProject = {
      tenant: 1,
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.',
      startDate: '2025-01-01',
      endDate: '2025-12-31',
      status: 'active',
      customerName: 'company',
      customerEmail: 'hrms@company.com',
      customerMobile: '+911234567890',
      lastChangeLogId: 1,
      isBlocked: false
    };

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/project/1',
      headers: { 
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: updatedProject
    });

    // Check the response
    expect(response.statusCode).toBe(403);
  });
});