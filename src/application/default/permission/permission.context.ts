import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { PermissionService } from "../permission/permission.service";
import { PermissionDao } from "../permission/dao/permission.dao";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";
import { UserDao } from "../user/dao/user.dao";

export default async function permissionContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('permissionDao', null);


  // Add Service decorators
  fastify.decorateRequest('permissionService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    const tenantId = (request.user as RequestUser).tenant;

    // DAO
    request.setDecorator<PermissionDao>(
      'permissionDao', 
      new PermissionDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), tenantId)
    );

    // Services
    request.setDecorator<PermissionService>(
      'permissionService', 
      new PermissionService(
        request.getDecorator<PermissionDao>('permissionDao'),
        request.getDecorator<UserDao>('userDao')
      )
    );
  });
}