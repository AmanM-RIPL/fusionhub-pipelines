import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { AccessPermission, FilterPermission, IColumnSchema, InsertableEntity, UpdateableEntity } from "../../common/types/entity";
import { NotFoundError } from "../../common/utils/custom-errors";
import { PermissionService } from "./permission.service";
import { IPermission } from "./permission.model";

export default async function permissionRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'permission-object',
    type: 'object',
    properties: {
      filterSchema: {
        type: 'object',
        properties: {
          project: { type: 'array', items: { type: 'number' } },
        }
      },
      accessSchema: {
        type: 'object',
        properties: {
          create: { type: 'boolean' },
          read: { anyOf: [{ type: 'boolean' }, { type: 'string', enum: ['self'] }] },
          update: { type: 'boolean' },
        },
        additionalProperties: false
       // required: ['create', 'read', 'update']
      },
      selectable: {
        type: 'object',
        properties: {
          id: { type: 'integer' },
          createdOn: { type: 'string', format: 'date-time' },
          user: { type: 'number' },
          project: { type: 'number' },
          entity: { type: 'string' },
          approval: { type: 'array', items: { type: 'number' } },
          filter: { $ref: 'permission-object#/properties/filterSchema' },
          access: { $ref: 'permission-object#/properties/accessSchema' },
        },
        required: ['id', 'createdOn', 'user','project', 'entity', 'approval', 'filter', 'access']
      },
      insertable: {
        type: 'object',
        properties: {
          user: { type: 'number' },
          entity: { type: 'string' },
          approval: { type: 'array', items: { type: 'number' } },
          filter: { $ref: 'permission-object#/properties/filterSchema' },
          access: { $ref: 'permission-object#/properties/accessSchema' },
        },
        required: ['user', 'entity', 'approval', 'filter', 'access']
      },
      updateable: {
        type: 'object',
        properties: {
          approval: { type: 'array', items: { type: 'number' } },
          filter: { $ref: 'permission-object#/properties/filterSchema' },
          access: { $ref: 'permission-object#/properties/accessSchema' },
        },
        required: ['approval', 'filter', 'access']
      }
    }
  });


  // Routes
  fastify.get(
    '/permission/:permissionId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get permission by id field',
        tags: ['permission'],
        summary: 'Get permission By ID',
        params: { 
          type: 'object', 
          properties: { 
            permissionId: { type: 'integer', minimum: 1 } 
          } 
        },
        response: {
          200: { $ref: 'permission-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { permissionId: number };
        }>,
        reply: FastifyReply
      ) => {
        const permission = await request.getDecorator<PermissionService>('permissionService').findById(request.params.permissionId);

        if (!permission) {
          throw new NotFoundError('Entity schema not found');
        }

        return reply.code(200).send(permission);
      }
    }
  );

  fastify.get(
    '/permission',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get permission all field',
        tags: ['permission'],
        summary: 'Get permission all',
        querystring: { 
          type: 'object', 
          properties: { 
            limit: { type: 'integer', default: 10, minimum: 1, maximum: 20 },
            offset: { type: 'integer', default: 0, minimum: 0 }
          }
        },
        response: {
          200: {
            type: 'array',
            items: { $ref: 'permission-object#/properties/selectable' }
          },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Querystring: { limit: number; offset: number };
        }>,
        reply: FastifyReply
      ) => {
        const permissions = await request.getDecorator<PermissionService>('permissionService').findAll(request.query.limit, request.query.offset);

        return reply.code(200).send(permissions);
      }
    }
  );

  fastify.post(
    '/permission',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Create permission field',
        tags: ['permission'],
        summary: 'Create permission',
        body: { $ref: 'permission-object#/properties/insertable' },
        response: {
          201: { $ref: 'permission-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission };
        }>,
        reply: FastifyReply
      ) => {
        const newEntitySchema: Omit<InsertableEntity<IPermission>, 'filter' | 'access'> & { filter: FilterPermission, access: AccessPermission } = request.body;

        const permission = await request.getDecorator<PermissionService>('permissionService').create(newEntitySchema);

        return reply.code(201).send(permission);
      }
    }
  );

  fastify.patch(
    '/permission/:permissionId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Update permission by id field',
        tags: ['permission'],
        summary: 'Update permission By ID',
        body: { $ref: 'permission-object#/properties/updateable' },
        params: {
          type: 'object',
          properties: {
            permissionId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'permission-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<InsertableEntity<IPermission>, 'filter' | 'access' | 'user' | 'entity'> & { filter: FilterPermission, access: AccessPermission };
          Params: { permissionId: number };
        }>,
        reply: FastifyReply
      ) => {
        const permission = await request.getDecorator<PermissionService>('permissionService').update(request.params.permissionId, request.body);

        if (!permission) {
          throw new NotFoundError('Entity schema not found');
        }

        return reply.code(200).send(permission);
      }
    }
  );

  fastify.delete(
    '/permission/:permissionId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Delete permission by id field',
        tags: ['permission'],
        summary: 'Delete permission By ID',
        params: {
          type: 'object',
          properties: {
            permissionId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'permission-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { permissionId: number };
        }>,
        reply: FastifyReply
      ) => {
        await request.getDecorator<PermissionService>('permissionService').delete(request.params.permissionId);

        return reply.code(204).send();
      }
    }
  );
}