import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { UserService } from "../../default/user/user.service";
import { NotFoundError } from "../../common/utils/custom-errors";
import dbTransactionPlugin from "../../../infrastructure/plugins/db-transaction-plugin";
import fp from 'fastify-plugin';
import { UserDao } from "../../default/user/dao/user.dao";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";

export default async function authRoutes(fastify: FastifyInstance) {

  // Registering the plugins
  await fastify.register(fp(dbTransactionPlugin));

  /* 
    Registering the context 
  */
  // Add DAO decorators
  fastify.decorateRequest('userDao', null);


  // Add Service decorators
  fastify.decorateRequest('userService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {

    // DAO
    request.setDecorator<UserDao>('userDao', new UserDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), null));

    // Services
    request.setDecorator<UserService>('userService', new UserService(request.getDecorator<UserDao>('userDao')));
  });
  
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
        required: ['id', 'username', 'email', 'firstName', 'lastName', 'mobile', 'createdOn']
      },
      authenticate: {
        type: 'object',
        properties: {
          username: { type: 'string' },
          password: { type: 'string' },
        },
        required: ['username', 'password']
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
        required: ['username', 'password', 'email', 'firstName', 'lastName', 'mobile']
      },
      updateable: {
        type: 'object',
        properties: {
          username: { type: 'string' },
          email: { type: 'string' },
          firstName: { type: 'string' },
          lastName: { type: 'string' },
          mobile: { type: 'string' }
        },
      },
    }
  });


  // Login route
  fastify.post(
    '/login',
    {
      schema: {
        body: { $ref: 'user-object#/properties/authenticate' },
        response: {
          201: {
            allOf: [
              { $ref: 'user-object#/properties/selectable' },
              {
                type: 'object',
                properties: {
                  token: { type: 'string' }
                },
                required: ['token']
              }
            ]
          },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: { username: string; password: string;};
        }>,
        reply: FastifyReply
      ) => {
        const userDetails = await request.getDecorator<UserService>('userService').authenticate(request.body.username, request.body.password);

        if (!userDetails) {
          throw new NotFoundError('Invalid username or password');
        }

        const token = fastify.jwt.sign({
          user: userDetails.id,
          tenant: userDetails.tenant
        });

        return reply.code(201).send({ ...userDetails, token });
      }
    }
  );
}