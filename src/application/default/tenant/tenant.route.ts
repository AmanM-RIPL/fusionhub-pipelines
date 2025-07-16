import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { Insertable, Selectable } from "kysely";
import { ITenant } from "./tenant.model";
import { UpdateableEntity } from "../../common/types/entity";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { TenantService } from "./tenant.service";
import { ForbiddenError, NotFoundError } from "../../common/utils/custom-errors";

export default async function tenantRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'tenant-object',
    type: 'object',
    properties: {
      selectable: {
        type: 'object',
        properties: {
          id: { type: 'integer' },
          name: { type: 'string' },
          createdOn: { type: 'string', format: 'date-time' },
          defaultEmail: { type: 'string' },
          defaultMobile: { type: 'string' },
          defaultEmail1: { type: 'string', nullable: true },
          defaultMobile1: { type: 'string', nullable: true },
          defaultEmail2: { type: 'string', nullable: true },
          isBlocked: { type: 'boolean' }
        },
        required: ['id', 'name', 'createdOn', 'defaultEmail', 'defaultMobile', 'isBlocked']
      },
      insertable: {
        type: 'object',
        properties: {
          name: { type: 'string' },
          defaultEmail: { type: 'string' },
          defaultMobile: { type: 'string' },
          defaultEmail1: { type: 'string', nullable: true },
          defaultMobile1: { type: 'string', nullable: true },
          defaultEmail2: { type: 'string', nullable: true },
          adminUsername: { type: 'string' },
          adminPassword: { type: 'string' }
        },
        required: ['name', 'defaultEmail', 'defaultMobile', 'adminUsername', 'adminPassword']
      },
      updateable: {
        type: 'object',
        properties: {
          name: { type: 'string' },
          defaultEmail: { type: 'string' },
          defaultMobile: { type: 'string' },
          defaultEmail1: { type: 'string' },
          defaultMobile1: { type: 'string' },
          defaultEmail2: { type: 'string' },
          isBlocked: { type: 'boolean' }
        },
      },
    }
  });


  // Routes
  fastify.get(
    '/tenant/:tenantId',
    {
      onRequest: fastify.authenticate,
      schema: {
        params: { 
          type: 'object', 
          properties: { 
            tenantId: { type: 'integer', minimum: 1 } 
          } 
        },
        response: {
          200: { $ref: 'tenant-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { tenantId: number };
        }>,
        reply: FastifyReply
      ) => {
        if ((request.user as RequestUser).user !== 1) {
          throw new ForbiddenError('Forbidden: Only admin can access this endpoint');
        }

        const tenantDetails = await request.getDecorator<TenantService>('tenantService').findById(request.params.tenantId);

        if (!tenantDetails) {
          throw new NotFoundError('Tenant not found');
        }

        return reply.code(200).send(tenantDetails);
      }
    }
  );

  fastify.get(
    '/tenant',
    {
      onRequest: fastify.authenticate,
      schema: {
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
            items: { $ref: 'tenant-object#/properties/selectable' }
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
        if ((request.user as RequestUser).user !== 1) {
          throw new ForbiddenError('Forbidden: Only admin can access this endpoint');
        }

        const tenantDetails = await request.getDecorator<TenantService>('tenantService').findAll(request.query.limit, request.query.offset);

        return reply.code(200).send(tenantDetails);
      }
    }
  );

  fastify.post(
    '/tenant',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'tenant-object#/properties/insertable' },
        response: {
          201: { $ref: 'tenant-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Insertable<ITenant> & { adminUsername: string, adminPassword: string };
        }>,
        reply: FastifyReply
      ) => {
        const newTenant: Insertable<ITenant> & { adminUsername: string, adminPassword: string } = request.body;

        if ((request.user as RequestUser).user !== 1) {
          throw new ForbiddenError('Forbidden: Only admin can access this endpoint');
        }

        const tenantDetails = await request.getDecorator<TenantService>('tenantService').create(newTenant);

        return reply.code(201).send(tenantDetails);
      }
    }
  );

  fastify.patch(
    '/tenant/:tenantId',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'tenant-object#/properties/updateable' },
        params: { 
          type: 'object', 
          properties: { 
            tenantId: { type: 'integer', minimum: 1 } 
          } 
        },
        response: {
          200: { $ref: 'tenant-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<UpdateableEntity<ITenant>, "createdOn">;
          Params: { tenantId: number };
        }>,
        reply: FastifyReply
      ) => {
        if ((request.user as RequestUser).user !== 1) {
          throw new ForbiddenError('Forbidden: Only admin can access this endpoint');
        }

        const tenantDetails = await request.getDecorator<TenantService>('tenantService').update(request.params.tenantId, request.body);

        if (!tenantDetails) {
          throw new NotFoundError('Tenant not found');
        }

        return reply.code(200).send(tenantDetails);
      }
    }
  );
}