import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { ChangeLogService } from "../change-log/change-log.service";
import { ChangeLogDao } from "../change-log/dao/change-log.dao";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";
import { ProjectDao } from "../project/dao/project.dao";
import { SQLiteSyncDao } from "./entities/dao/sqlite-sync.dao";

export default async function changeLogContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('changeLogDao', null);


  // Add Service decorators
  fastify.decorateRequest('changeLogService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    const tenantId = (request.user as RequestUser).tenant;
    const userId = (request.user as RequestUser).user;

    // DAO
    request.setDecorator<ChangeLogDao>(
      'changeLogDao',
      new ChangeLogDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), tenantId, userId)
    );

    // Services
    request.setDecorator<ChangeLogService>(
      'changeLogService',
      new ChangeLogService(
        request.getDecorator<ChangeLogDao>('changeLogDao'),
        request.getDecorator<ProjectDao>('projectDao'),
        userId
      )
    );
  });
}