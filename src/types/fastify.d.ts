import { FastifyPostgres } from "fastify-postgres";
import { Kysely } from "kysely";
import { IDatabase } from "../infrastructure/db/kysely/types";

declare module 'fastify' {
  export interface FastifyInstance {
    pg: FastifyPostgres;
    db: Kysely<IDatabase>;
    authenticate: (request: FastifyRequest, reply: FastifyReply) => Promise<void>;
    platformVersion?: string;
  }

  interface FastifyInstance extends 
  FastifyJwtNamespace<{
    jwtDecode: 'securityJwtDecode',
    jwtSign: 'securityJwtSign',
    jwtVerify: 'securityJwtVerify',
  }> { }
}