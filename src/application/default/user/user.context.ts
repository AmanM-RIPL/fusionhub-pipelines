import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { UserService } from "../user/user.service";
import { UserDao } from "../user/dao/user.dao";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";

export default async function userContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('userDao', null);


  // Add Service decorators
  fastify.decorateRequest('userService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    const tenantId = (request.user as RequestUser).tenant;

    // DAO
    request.setDecorator<UserDao>('userDao', new UserDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), tenantId));

    // Services
    request.setDecorator<UserService>('userService', new UserService(request.getDecorator<UserDao>('userDao')));
  });
}