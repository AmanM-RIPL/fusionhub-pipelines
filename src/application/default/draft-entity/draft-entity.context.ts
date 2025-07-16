import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { DraftEntityService } from "../draft-entity/draft-entity.service";
import { DraftEntityDao } from "../draft-entity/dao/draft-entity.dao";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";

export default async function draftEntityContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('draftEntityDao', null);


  // Add Service decorators
  fastify.decorateRequest('draftEntityService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    const tenantId = (request.user as RequestUser).tenant;
    const userId = (request.user as RequestUser).user;

    // DAO
    request.setDecorator<DraftEntityDao>(
      'draftEntityDao', 
      new DraftEntityDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction'), tenantId)
    );

    // Services
    request.setDecorator<DraftEntityService>(
      'draftEntityService', 
      new DraftEntityService(
        request.getDecorator<DraftEntityDao>('draftEntityDao'),
        userId
      )
    );
  });
}