import { FastifyInstance } from "fastify";
import tenantRoutes from "../tenant/tenant.route";
import projectRoutes from "../project/project.route";
import userRoutes from "../user/user.route";
import tenantContextPlugin from "../tenant/tenant.context";
import projectContextPlugin from "../project/project.context";
import userContextPlugin from "../user/user.context";
import dbTransactionPlugin from "../../../infrastructure/plugins/db-transaction-plugin";
import fp from 'fastify-plugin';
import permissionContextPlugin from "../permission/permission.context";
import permissionRoutes from "../permission/permission.route";
import draftEntityContextPlugin from "../draft-entity/draft-entity.context";
import draftEntityRoutes from "../draft-entity/draft-entity.route";
import changeLogContextPlugin from "../change-log/change-log.context";
import changeLogRoutes from "../change-log/change-log.route";

export default async function defaultRoutes(fastify: FastifyInstance) {

  // Registering the plugins
  await fastify.register(fp(dbTransactionPlugin));
  await fastify.register(fp(draftEntityContextPlugin));
  
  await fastify.register(fp(tenantContextPlugin));
  await fastify.register(fp(projectContextPlugin));
  await fastify.register(fp(changeLogContextPlugin));
  await fastify.register(fp(userContextPlugin));
  await fastify.register(fp(permissionContextPlugin));

  // Registering routes
  await fastify.register(fp(tenantRoutes));
  await fastify.register(fp(projectRoutes));
  await fastify.register(fp(userRoutes));
  await fastify.register(fp(permissionRoutes));
  await fastify.register(fp(draftEntityRoutes));
  await fastify.register(fp(changeLogRoutes));
}