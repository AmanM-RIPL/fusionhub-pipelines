import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { ProjectService } from "../project/project.service";
import { ProjectDao } from "../project/dao/project.dao";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { UserDao } from "../user/dao/user.dao";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";

export default async function projectContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('projectDao', null);


  // Add Service decorators
  fastify.decorateRequest('projectService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    const tenant = (request.user as RequestUser).tenant;
    // DAO
    request.setDecorator<ProjectDao>('projectDao', new ProjectDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), tenant));

    // Services
    request.setDecorator<ProjectService>('projectService', new ProjectService(request.getDecorator<ProjectDao>('projectDao'), request.getDecorator<UserDao>('userDao')));
  });
}