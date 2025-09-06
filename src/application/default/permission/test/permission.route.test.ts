import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { PermissionService } from "../permission.service";
import permissionRoutes from "../permission.route";
import { Insertable, Selectable } from "kysely";
import { IPermission } from "../permission.model";
import { AccessPermission, FilterPermission, InsertableEntity } from "../../../common/types/entity";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";

describe('Default -> Permission -> PermissionRoute', () => {
  let fastify: FastifyInstance;
  let permissionService: DeepMockProxy<PermissionService>;

  beforeAll(async () => {
      fastify = Fastify({ logger: false }) as unknown as FastifyInstance;
      permissionService = mockDeep<PermissionService>();

      // Mock the permissionService methods
      fastify.decorateRequest('permissionService', null);
      fastify.addHook('onRequest', async (request, reply) => {
          request.user = { user: 1, tenant: 1 } as RequestUser;
          request.setDecorator<PermissionService>('permissionService', permissionService);
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

      await permissionRoutes(fastify as FastifyInstance);
      await fastify.ready();
  });

  afterAll(async () => {
      await fastify.close();
      jest.clearAllMocks();
  });

  test('should register permission routes', async () => {
      const routes = fastify.printRoutes();
      expect(routes).toContain(':permissionId');
      expect(routes).toContain('permission');
  });

  test('should handle get permission by id', async () => {
      const mockPermission = {
          id: 13,
          tenant: 1,
          createdOn: new Date(),
          user: 1,
          project: 2,
          entity: "employee",
          permissionType: 'READ' as const,
          approval: [2, 3],
          filter: {
              project: [1, 2, 3],
              subProject: [1, 2, 3],
              businessEntityBranch: [1, 2, 3],
              financialYear: [1, 2, 3],
          } as FilterPermission,
          access: {
              create: true,
              read: true,
              update: false
          } as AccessPermission
      } as Selectable<IPermission>;

      permissionService.findById.mockResolvedValue(mockPermission);

      const response = await fastify.inject({
          method: 'GET',
          url: '/permission/13',
          headers: { authorization: 'Bearer test-token' }
      });

      expect(response.statusCode).toBe(200);

      const result = response.body;
      const parsed = JSON.parse(result);
      expect(parsed.id).toEqual(13);
      expect(parsed.entity).toEqual('employee');
      expect(parsed.user).toEqual(1);
      expect(parsed.approval).toEqual([2, 3]);
      expect(parsed.filter).toEqual({ project: [1, 2, 3], subProject: [1, 2, 3], businessEntityBranch: [1, 2, 3], financialYear: [1, 2, 3] });
      expect(parsed.access).toEqual({ create: true, read: true, update: false });
      expect(permissionService.findById).toHaveBeenCalledWith(13);
  });

  test('should handle get all permissions', async () => {
      const mockPermissions = [
          {
              id: 13,
              tenant: 1,
              createdOn: new Date("2025-08-28T07:21:33.998Z"),
              user: 1,
              project: 2,
              entity: "employee",
              permissionType: 'CREATE' as const,
              approval: [2],
              filter: {
                  project: [1, 2],
                  subProject: [1, 2, 3],
                  businessEntityBranch: [1, 2, 3],
                  financialYear: [1, 2, 3],
              } as FilterPermission,
              access: {
                  create: true,
                  read: 'self',
                  update: true
              } as AccessPermission
          }
      ] as Selectable<IPermission>[];

      permissionService.findAll.mockResolvedValue(mockPermissions);

      const response = await fastify.inject({
          method: 'GET',
          url: '/permission',
          headers: { authorization: 'Bearer test-token' },
          query: { limit: '10', offset: '0' }
      });

      expect(response.statusCode).toBe(200);

      const result = response.body;
      const parsed = JSON.parse(result);
      expect(Array.isArray(parsed)).toBe(true);
      expect(parsed).toHaveLength(1);

      const firstPermission = parsed[0];
      expect(firstPermission.entity).toEqual('employee');
      expect(firstPermission.user).toEqual(1);
      expect(firstPermission.id).toEqual(13);
      expect(firstPermission.approval).toEqual([2]);
      expect(firstPermission.filter).toEqual({ project: [1, 2], subProject: [1, 2, 3], businessEntityBranch: [1, 2, 3], financialYear: [1, 2, 3] });
      expect(firstPermission.access).toEqual({ create: true, read: 'self', update: true });
      expect(firstPermission.createdOn).toEqual("2025-08-28T07:21:33.998Z");
      expect(permissionService.findAll).toHaveBeenCalledWith(10, 0);
  });

  test('should handle create permission', async () => {
      const requestPayload = {
          user: 1,
          entity: "employee",
          approval: [501, 502],
          filter: {
              project: [1, 2, 3],
              subProject: [1, 2, 3],
              businessEntityBranch: [1, 2, 3],
              financialYear: [1, 2, 3],
          } as FilterPermission,
          access: {
              create: true,
              read: true,
              update: true
          } as AccessPermission
      };

      const createdPermission: Selectable<IPermission> = {
          id: 2,
          tenant: 1,
          createdOn: new Date(),
          user: 1,
          project: 201,
          entity: "employee",
          permissionType: 'CREATE',
          approval: [501, 502],
          filter: {
              project: [1, 2, 3],
              subProject: [1, 2, 3],
              businessEntityBranch: [1, 2, 3],
              financialYear: [1, 2, 3],
          } as FilterPermission,
          access: {
              create: true,
              read: true,
              update: true
          } as AccessPermission
      };

      permissionService.create.mockResolvedValue(createdPermission);

      const response = await fastify.inject({
          method: 'POST',
          url: '/permission',
          headers: {
              authorization: 'Bearer test-token',
              'content-type': 'application/json'
          },
          payload: requestPayload
      });
      expect(response.statusCode).toBe(201);
      expect(JSON.parse(response.body)).toEqual({
          ...createdPermission,
          createdOn: createdPermission.createdOn.toISOString()
      });
      expect(permissionService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle update permission', async () => {
      const updatedPermission = {
          approval: [3, 4],
          filter: {
              project: [2, 3],
              subProject: [2, 3],
              businessEntityBranch: [2, 3],
              financialYear: [2, 3],
          } as FilterPermission,
          access: {
              create: false,
              read: true,
              update: false
          } as AccessPermission
      };

      const updatedPermissionResult: Selectable<IPermission> = {
          id: 13,
          tenant: 1,
          createdOn: new Date(),
          user: 1,
          project: 2,
          entity: "employee",
          permissionType: 'READ',
          approval: [3, 4],
          filter: {
              project: [2, 3],
              subProject: [2, 3],
              businessEntityBranch: [2, 3],
              financialYear: [2, 3],
          } as FilterPermission,
          access: {
              create: false,
              read: true,
              update: false
          } as AccessPermission
      };

      permissionService.update.mockResolvedValue(updatedPermissionResult);

      const response = await fastify.inject({
          method: 'PATCH',
          url: '/permission/13',
          headers: {
              authorization: 'Bearer test-token',
              'content-type': 'application/json'
          },
          payload: updatedPermission
      });

      expect(response.statusCode).toBe(200);
      expect(JSON.parse(response.body)).toEqual({
          ...updatedPermissionResult,
          createdOn: updatedPermissionResult.createdOn.toISOString()
      });
      expect(permissionService.update).toHaveBeenCalledWith(13, updatedPermission);
  });

  test('should handle permission not found', async () => {
      permissionService.findById.mockResolvedValue(undefined);

      const response = await fastify.inject({
          method: 'GET',
          url: '/permission/999',
          headers: { authorization: 'Bearer test-token' }
      });

      expect(response.statusCode).toBe(404);
      expect(JSON.parse(response.body)).toEqual({ error: 'Entity schema not found' });
      expect(permissionService.findById).toHaveBeenCalledWith(999);
  });

  test('should handle permission creation db error', async () => {
      const requestPayload = {
          user: 1,
          entity: "employee",
          approval: [501],
          filter: {
              project: []
          } as FilterPermission,
          access: {
              create: true,
              read: false,
              update: false
          } as AccessPermission
      };

      permissionService.create.mockRejectedValue(new Error('Database error'));

      const response = await fastify.inject({
          method: 'POST',
          url: '/permission',
          headers: {
              authorization: 'Bearer test-token',
              'content-type': 'application/json'
          },
          payload: requestPayload
      });

      expect(response.statusCode).toBe(500);
      expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
      expect(permissionService.create).toHaveBeenCalledWith(requestPayload);
  });

  test('should handle permission update not found', async () => {
      const updatedPermission = {
          approval: [501, 502],
          filter: {
              project: [1]
          } as FilterPermission,
          access: {
              create: false,
              read: 'self',
              update: true
          } as AccessPermission
      };

      permissionService.update.mockResolvedValue(undefined);

      const response = await fastify.inject({
          method: 'PATCH',
          url: '/permission/999',
          headers: {
              authorization: 'Bearer test-token',
              'content-type': 'application/json'
          },
          payload: updatedPermission
      });

      expect(response.statusCode).toBe(404);
      expect(JSON.parse(response.body)).toEqual({ error: 'Entity schema not found' });
  });

  test('should handle permission update db error', async () => {
      const updatedPermission = {
          approval: [501, 502],
          filter: {
              project: [1]
          } as FilterPermission,
          access: {
              create: false,
              read: 'self',
              update: true
          } as AccessPermission
      };

      permissionService.update.mockRejectedValue(new Error('Database error'));

      const response = await fastify.inject({
          method: 'PATCH',
          url: '/permission/1',
          headers: {
              authorization: 'Bearer test-token',
              'content-type': 'application/json'
          },
          payload: updatedPermission
      });

      expect(response.statusCode).toBe(500);
      expect(JSON.parse(response.body)).toEqual({ error: 'Internal Server Error' });
  });

  test('should successfully delete permission', async () => {
      permissionService.delete.mockResolvedValue(undefined);

      const response = await fastify.inject({
          method: 'DELETE',
          url: '/permission/13',
          headers: { authorization: 'Bearer test-token' }
      });

      expect(response.statusCode).toBe(204);
      expect(response.body).toBe('');
      expect(permissionService.delete).toHaveBeenCalledWith(13);
  });

  test('should handle delete permission error', async () => {
      permissionService.delete.mockRejectedValue(
          new Error("Cannot delete permission due to constraints.")
      );

      const response = await fastify.inject({
          method: 'DELETE',
          url: '/permission/13',
          headers: { authorization: 'Bearer test-token' }
      });

      expect(response.statusCode).toBe(500);
      const result = JSON.parse(response.body);
      expect(result.error).toBe("Internal Server Error");
  });
});

describe('Default -> Permission -> PermissionRoute : Forbidden', () => {
  let fastify: FastifyInstance;
  let permissionService: DeepMockProxy<PermissionService>;

  beforeAll(async () => {
    fastify = Fastify() as unknown as FastifyInstance;
    permissionService = mockDeep<PermissionService>();

    // Mock the permissionService methods
    fastify.decorateRequest('permissionService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      request.setDecorator<PermissionService>('permissionService', permissionService);
    });

    // Mock authenticate method that throws forbidden
    fastify.decorate('authenticate', async (request: any, reply: any) => {
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

    await permissionRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  test('should handle get permission by id if Forbidden', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/permission/1',
      headers: { authorization: 'Bearer test-token' }
    });
    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle get all permissions if Forbidden', async () => {
    const response = await fastify.inject({
      method: 'GET',
      url: '/permission',
      headers: { authorization: 'Bearer test-token' },
      query: { limit: '10', offset: '0' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle create permission if Forbidden', async () => {
    const requestPayload = {
      user: 1,
      entity: "employee",
      approval: [501, 502],
      filter: {
        project: [1, 2]
      } as FilterPermission,
      access: {
        create: true,
        read: false,
        update: false
      } as AccessPermission
    };

    const response = await fastify.inject({
      method: 'POST',
      url: '/permission',
      headers: {
          authorization: 'Bearer test-token',
          'content-type': 'application/json'
      },
      payload: requestPayload
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle update permission if Forbidden', async () => {
    const updatedPermission = {
      approval: [501, 502],
      filter: {
        project: [1]
      } as FilterPermission,
      access: {
        create: false,
        read: 'self',
        update: true
      } as AccessPermission
    };

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/permission/1',
      headers: {
        authorization: 'Bearer test-token',
        'content-type': 'application/json'
      },
      payload: updatedPermission
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

  test('should handle delete permission if Forbidden', async () => {
    const response = await fastify.inject({
      method: 'DELETE',
      url: '/permission/1',
      headers: { authorization: 'Bearer test-token' }
    });

    expect(response.statusCode).toBe(403);
    expect(JSON.parse(response.body)).toEqual({ error: 'Forbidden' });
  });

});