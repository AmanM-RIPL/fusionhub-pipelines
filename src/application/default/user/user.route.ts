import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { Insertable, Selectable } from "kysely";
import { IUser } from "./user.model";
import { UpdateableEntity } from "../../common/types/entity";
import { UserService } from "./user.service";
import { NotFoundError } from "../../common/utils/custom-errors";

export default async function userRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'user-object',
    type: 'object',
    properties: {
      selectable: {
        type: 'object',
        properties: {
          id: { type: 'integer' },
          createdOn: { type: 'string', format: 'date-time' },
          username: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
        required: ['id', 'username', 'email', 'firstName', 'lastName', 'mobile', 'createdOn'],
      },
      adminSelectable: {
        type: 'object',
        properties: {
          id: { type: 'integer' },
          createdOn: { type: 'string', format: 'date-time' },
          username: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
        required: ['id', 'username', 'email', 'firstName', 'lastName', 'mobile', 'createdOn'],
      },
      authenticate: {
        type: 'object',
        properties: {
          username: { type: 'string' },
          password: { type: 'string' },
        },
        required: ['username', 'password'],
      },
      insertable: {
        type: 'object',
        properties: {
          username: { type: 'string' },
          password: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
        required: ['username', 'password', 'email', 'firstName', 'lastName', 'mobile'],
      },
      adminInsertable: {
        type: 'object',
        properties: {
          tenant: { type: 'integer' },
          username: { type: 'string' },
          password: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
        required: ['tenant', 'username', 'password', 'email', 'firstName', 'lastName', 'mobile'],
      },
      updateable: {
        type: 'object',
        properties: {
          username: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
      },
      adminUpdateable: {
        type: 'object',
        properties: {
          tenant: { type: 'integer' },
          username: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' },
        },
      },
    },
  });



  // Routes
  fastify.get(
    '/user/:userId',
    {
      onRequest: fastify.authenticate,
      schema: {
        params: {
          type: 'object',
          properties: {
            userId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'user-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { userId: number };
        }>,
        reply: FastifyReply
      ) => {
        const userDetails = await request.getDecorator<UserService>('userService').findById(request.params.userId);

        if (!userDetails) {
          throw new NotFoundError('User not found');
        }

        return reply.code(200).send(userDetails);
      }
    }
  );

  fastify.get(
    '/user',
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
            items: { $ref: 'user-object#/properties/selectable' }
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
        const userDetails = await request.getDecorator<UserService>('userService').findAll(request.query.limit, request.query.offset);

        return reply.code(200).send(userDetails);
      }
    }
  );

  fastify.post(
    '/user',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'user-object#/properties/insertable' },
        response: {
          201: { $ref: 'user-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Insertable<IUser>;
        }>,
        reply: FastifyReply
      ) => {
        const newTenant: Insertable<IUser> = request.body;

        const userDetails = await request.getDecorator<UserService>('userService').create(newTenant);

        return reply.code(201).send(userDetails);
      }
    }
  );

  fastify.post(
    '/admin-user',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'user-object#/properties/adminInsertable' },
        response: {
          201: { $ref: 'user-object#/properties/adminSelectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Insertable<IUser>;
        }>,
        reply: FastifyReply
      ) => {
        const newTenant: Insertable<IUser> = request.body;

        const adminUserDetails = await request.getDecorator<UserService>('userService').adminCreate(newTenant);

        return reply.code(201).send(adminUserDetails);
      }
    }
  );

  fastify.patch(
    '/user/:userId',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'user-object#/properties/updateable' },
        params: {
          type: 'object',
          properties: {
            userId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'user-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: UpdateableEntity<IUser>;
          Params: { userId: number };
        }>,
        reply: FastifyReply
      ) => {
        const userDetails = await request.getDecorator<UserService>('userService').update(request.params.userId, request.body);

        if (!userDetails) {
          throw new NotFoundError('User not found');
        }

        return reply.code(200).send(userDetails);
      }
    }
  );
}