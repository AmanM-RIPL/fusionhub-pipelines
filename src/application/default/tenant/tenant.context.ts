import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { TenantService } from "../tenant/tenant.service";
import { TenantDao } from "../tenant/dao/tenant.dao";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../../infrastructure/db/kysely/types";

export default async function tenantContextPlugin(fastify: FastifyInstance) {

  // Add DAO decorators
  fastify.decorateRequest('tenantDao', null);
   

  // Add Service decorators
  fastify.decorateRequest('tenantService', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    // DAO
    request.setDecorator<TenantDao>('tenantDao', new TenantDao(request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction')));

    // Services
     request.setDecorator<TenantService>('tenantService', new TenantService(request.getDecorator<TenantDao>('tenantDao'))); 
     
    });
}